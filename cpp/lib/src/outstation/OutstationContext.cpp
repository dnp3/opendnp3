/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "OutstationContext.h"

#include "app/APDUBuilders.h"
#include "app/APDULogging.h"
#include "app/Functions.h"
#include "app/parsing/APDUHeaderParser.h"
#include "app/parsing/APDUParser.h"
#include "logging/LogMacros.h"
#include "outstation/AssignClassHandler.h"
#include "outstation/ClassBasedRequestHandler.h"
#include "outstation/CommandActionAdapter.h"
#include "outstation/CommandResponseHandler.h"
#include "outstation/ConstantCommandAction.h"
#include "outstation/FreezeRequestHandler.h"
#include "outstation/IINHelpers.h"
#include "outstation/ReadHandler.h"
#include "outstation/WriteHandler.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

OContext::OContext(const Addresses& addresses,
                   const OutstationConfig& config,
                   const DatabaseConfig& db_config,
                   const Logger& logger,
                   const std::shared_ptr<exe4cpp::IExecutor>& executor,
                   std::shared_ptr<ILowerLayer> lower,
                   std::shared_ptr<ICommandHandler> commandHandler,
                   std::shared_ptr<IOutstationApplication> application)
    :

      addresses(addresses),
      logger(logger),
      executor(executor),
      lower(std::move(lower)),
      commandHandler(std::move(commandHandler)),
      application(std::move(application)),
      eventBuffer(config.eventBufferConfig),
      database(db_config, eventBuffer, *this->application, config.params.typesAllowedInClass0),
      rspContext(database, eventBuffer),
      params(config.params),
      isOnline(false),
      isTransmitting(false),
      staticIIN(IINBit::DEVICE_RESTART),
      deferred(config.params.maxRxFragSize),
      sol(config.params.maxTxFragSize),
      unsol(config.params.maxTxFragSize),
      unsolRetries(config.params.numUnsolRetries),
      shouldCheckForUnsolicited(false)
{
}

bool OContext::OnLowerLayerUp()
{
    if (isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "already online");
        return false;
    }

    isOnline = true;
    this->shouldCheckForUnsolicited = true;
    this->CheckForTaskStart();
    return true;
}

bool OContext::OnLowerLayerDown()
{
    if (!isOnline)
    {
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "already offline");
        return false;
    }

    this->state = &StateIdle::Inst();

    isOnline = false;
    isTransmitting = false;

    sol.Reset();
    unsol.Reset();
    history.Reset();
    deferred.Reset();
    eventBuffer.Unselect();
    rspContext.Reset();
    confirmTimer.cancel();

    return true;
}

bool OContext::OnTxReady()
{
    if (!isOnline || !isTransmitting)
    {
        return false;
    }

    this->isTransmitting = false;
    this->CheckForTaskStart();
    return true;
}

bool OContext::OnReceive(const Message& message)
{
    if (!this->isOnline)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "ignoring received data while offline");
        return false;
    }

    this->ProcessMessage(message);

    this->CheckForTaskStart();

    return true;
}

OutstationState& OContext::OnReceiveSolRequest(const ParsedRequest& request)
{
    // analyze this request to see how it compares to the last request
    if (this->history.HasLastRequest())
    {
        if (this->sol.seq.num.Equals(request.header.control.SEQ))
        {
            if (this->history.FullyEqualsLastRequest(request.header, request.objects))
            {
                if (request.header.function == FunctionCode::READ)
                {
                    return this->state->OnRepeatReadRequest(*this, request);
                }

                return this->state->OnRepeatNonReadRequest(*this, request);
            }
            else // new operation with same SEQ
            {
                return this->ProcessNewRequest(request);
            }
        }
        else // completely new sequence #
        {
            return this->ProcessNewRequest(request);
        }
    }
    else
    {
        return this->ProcessNewRequest(request);
    }
}

OutstationState& OContext::ProcessNewRequest(const ParsedRequest& request)
{
    this->sol.seq.num = request.header.control.SEQ;

    if (request.header.function == FunctionCode::READ)
    {
        return this->state->OnNewReadRequest(*this, request);
    }

    return this->state->OnNewNonReadRequest(*this, request);
}

bool OContext::ProcessObjects(const ParsedRequest& request)
{
    if (request.addresses.IsBroadcast())
    {
        this->state = &this->state->OnBroadcastMessage(*this, request);
        return true;
    }

    if (Functions::IsNoAckFuncCode(request.header.function))
    {
        // this is the only request we process while we are transmitting
        // because it doesn't require a response of any kind
        return this->ProcessRequestNoAck(request);
    }

    if (this->isTransmitting)
    {
        this->deferred.Set(request);
        return true;
    }

    if (request.header.function == FunctionCode::CONFIRM)
    {
        return this->ProcessConfirm(request);
    }

    return this->ProcessRequest(request);
}

bool OContext::ProcessRequest(const ParsedRequest& request)
{
    if (request.header.control.UNS)
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring unsol with invalid function code: %s",
                         FunctionCodeSpec::to_human_string(request.header.function));
        return false;
    }

    this->state = &this->OnReceiveSolRequest(request);
    return true;
}

bool OContext::ProcessConfirm(const ParsedRequest& request)
{
    this->state = &this->state->OnConfirm(*this, request);
    return true;
}

OutstationState& OContext::BeginResponseTx(uint16_t destination, APDUResponse& response)
{
    CheckForBroadcastConfirmation(response);

    const auto data = response.ToRSeq();
    this->sol.tx.Record(response.GetControl(), data);
    this->sol.seq.confirmNum = response.GetControl().SEQ;
    this->BeginTx(destination, data);

    if (response.GetControl().CON)
    {
        this->RestartSolConfirmTimer();
        return StateSolicitedConfirmWait::Inst();
    }

    return StateIdle::Inst();
}

void OContext::BeginRetransmitLastResponse(uint16_t destination)
{
    this->BeginTx(destination, this->sol.tx.GetLastResponse());
}

void OContext::BeginRetransmitLastUnsolicitedResponse()
{
    this->BeginTx(this->addresses.destination, this->unsol.tx.GetLastResponse());
}

void OContext::BeginUnsolTx(APDUResponse& response)
{
    CheckForBroadcastConfirmation(response);

    const auto data = response.ToRSeq();
    this->unsol.tx.Record(response.GetControl(), data);
    this->unsol.seq.confirmNum = this->unsol.seq.num;
    this->unsol.seq.num.Increment();
    this->BeginTx(this->addresses.destination, data);
}

void OContext::BeginTx(uint16_t destination, const ser4cpp::rseq_t& message)
{
    logging::ParseAndLogResponseTx(this->logger, message);
    this->isTransmitting = true;
    this->lower->BeginTransmit(Message(Addresses(this->addresses.source, destination), message));
}

void OContext::CheckForTaskStart()
{
    // do these checks in order of priority
    this->CheckForDeferredRequest();
    this->CheckForUnsolicitedNull();
    if (this->shouldCheckForUnsolicited)
    {
        this->CheckForUnsolicited();
    }
}

void OContext::CheckForDeferredRequest()
{
    if (this->CanTransmit() && this->deferred.IsSet())
    {
        auto handler = [this](const ParsedRequest& request) { return this->ProcessDeferredRequest(request); };
        this->deferred.Process(handler);
    }
}

void OContext::CheckForUnsolicitedNull()
{
    if (this->CanTransmit() && this->state->IsIdle() && this->params.allowUnsolicited)
    {
        if (!this->unsol.completedNull)
        {
            // send a NULL unsolcited message
            auto response = this->unsol.tx.Start();
            build::NullUnsolicited(response, this->unsol.seq.num, this->GetResponseIIN());
            this->RestartUnsolConfirmTimer();
            this->state = this->params.noDefferedReadDuringUnsolicitedNullResponse
                ? &StateNullUnsolicitedConfirmWait::Inst()
                : &StateUnsolicitedConfirmWait::Inst();
            this->BeginUnsolTx(response);
        }
    }
}

void OContext::CheckForUnsolicited()
{
    if (this->shouldCheckForUnsolicited && this->CanTransmit() && this->state->IsIdle()
        && this->params.allowUnsolicited)
    {
        this->shouldCheckForUnsolicited = false;

        if (this->unsol.completedNull)
        {
            // are there events to be reported?
            if (this->params.unsolClassMask.Intersects(this->eventBuffer.UnwrittenClassField()))
            {

                auto response = this->unsol.tx.Start();
                auto writer = response.GetWriter();

                this->unsolRetries.Reset();
                this->eventBuffer.Unselect();
                this->eventBuffer.SelectAllByClass(this->params.unsolClassMask);
                this->eventBuffer.Load(writer);

                build::NullUnsolicited(response, this->unsol.seq.num, this->GetResponseIIN());
                this->RestartUnsolConfirmTimer();
                this->state = &StateUnsolicitedConfirmWait::Inst();
                this->BeginUnsolTx(response);
            }
        }
    }
}

bool OContext::ProcessDeferredRequest(const ParsedRequest& request)
{
    if (request.header.function == FunctionCode::CONFIRM)
    {
        this->ProcessConfirm(request);
        return true;
    }

    if (request.header.function == FunctionCode::READ)
    {
        if (this->state->IsIdle())
        {
            this->ProcessRequest(request);
            return true;
        }

        return false;
    }
    else
    {
        this->ProcessRequest(request);
        return true;
    }
}

void OContext::RestartSolConfirmTimer()
{
    auto timeout = [&]() {
        this->state = &this->state->OnConfirmTimeout(*this);
        this->CheckForTaskStart();
    };

    this->confirmTimer.cancel();
    this->confirmTimer = this->executor->start(this->params.solConfirmTimeout.value, timeout);
}

void OContext::RestartUnsolConfirmTimer()
{
    auto timeout = [&]() {
        this->state = &this->state->OnConfirmTimeout(*this);
        this->CheckForTaskStart();
    };

    this->confirmTimer.cancel();
    this->confirmTimer = this->executor->start(this->params.unsolConfirmTimeout.value, timeout);
}

OutstationState& OContext::RespondToNonReadRequest(const ParsedRequest& request)
{
    this->history.RecordLastProcessedRequest(request.header, request.objects);

    auto response = this->sol.tx.Start();
    auto writer = response.GetWriter();
    response.SetFunction(FunctionCode::RESPONSE);
    response.SetControl(AppControlField(true, true, false, false, request.header.control.SEQ));
    auto iin = this->HandleNonReadResponse(request.header, request.objects, writer);
    response.SetIIN(iin | this->GetResponseIIN());
    return this->BeginResponseTx(request.addresses.source, response);
}

OutstationState& OContext::RespondToReadRequest(const ParsedRequest& request)
{
    this->history.RecordLastProcessedRequest(request.header, request.objects);

    auto response = this->sol.tx.Start();
    auto writer = response.GetWriter();
    response.SetFunction(FunctionCode::RESPONSE);
    auto result = this->HandleRead(request.objects, writer);
    result.second.SEQ = request.header.control.SEQ;
    response.SetControl(result.second);
    response.SetIIN(result.first | this->GetResponseIIN());

    return this->BeginResponseTx(request.addresses.source, response);
}

OutstationState& OContext::ContinueMultiFragResponse(const Addresses& addresses, const AppSeqNum& seq)
{
    auto response = this->sol.tx.Start();
    auto writer = response.GetWriter();
    response.SetFunction(FunctionCode::RESPONSE);
    auto control = this->rspContext.LoadResponse(writer);
    control.SEQ = seq;
    response.SetControl(control);
    response.SetIIN(this->GetResponseIIN());

    return this->BeginResponseTx(addresses.source, response);
}

bool OContext::CanTransmit() const
{
    return isOnline && !isTransmitting;
}

IINField OContext::GetResponseIIN()
{
    return this->staticIIN | this->GetDynamicIIN() | this->application->GetApplicationIIN().ToIIN();
}

IINField OContext::GetDynamicIIN()
{
    auto classField = this->eventBuffer.UnwrittenClassField();

    IINField ret;
    ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
    ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
    ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
    ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, this->eventBuffer.IsOverflown());

    return ret;
}

void OContext::UpdateLastBroadcastMessageReceived(uint16_t destination)
{
    switch (destination)
    {
    case LinkBroadcastAddress::DontConfirm:
        lastBroadcastMessageReceived.set(LinkBroadcastAddress::DontConfirm);
        break;
    case LinkBroadcastAddress::ShallConfirm:
        lastBroadcastMessageReceived.set(LinkBroadcastAddress::ShallConfirm);
        break;
    case LinkBroadcastAddress::OptionalConfirm:
        lastBroadcastMessageReceived.set(LinkBroadcastAddress::OptionalConfirm);
        break;
    default:
        lastBroadcastMessageReceived.clear();
    }
}

void OContext::CheckForBroadcastConfirmation(APDUResponse& response)
{
    if (lastBroadcastMessageReceived.is_set())
    {
        response.SetIIN(response.GetIIN() | IINField(IINBit::BROADCAST));

        if (lastBroadcastMessageReceived.get() != LinkBroadcastAddress::ShallConfirm)
        {
            lastBroadcastMessageReceived.clear();
        }
        else
        {
            // The broadcast address requested a confirmation
            auto control = response.GetControl();
            control.CON = true;
            response.SetControl(control);
        }
    }
}

bool OContext::ProcessMessage(const Message& message)
{
    // is the message addressed to this outstation
    if (message.addresses.destination != this->addresses.source && !message.addresses.IsBroadcast())
    {
        return false;
    }

    // is the message coming from the expected master?
    if (!this->params.respondToAnyMaster && (message.addresses.source != this->addresses.destination))
    {
        return false;
    }

    FORMAT_HEX_BLOCK(this->logger, flags::APP_HEX_RX, message.payload, 18, 18);

    if (message.addresses.IsBroadcast())
    {
        UpdateLastBroadcastMessageReceived(message.addresses.destination);
    }

    const auto result = APDUHeaderParser::ParseRequest(message.payload, &this->logger);
    if (!result.success)
    {
        return false;
    }

    logging::LogHeader(this->logger, flags::APP_HEADER_RX, result.header);

    if (!result.header.control.IsFirAndFin())
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring fragment. Requests must have FIR/FIN == 1");
        return false;
    }

    if (result.header.control.CON)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring fragment. Requests cannot request confirmation");
        return false;
    }

    return this->ProcessObjects(ParsedRequest(message.addresses, result.header, result.objects));
}

void OContext::HandleNewEvents()
{
    this->shouldCheckForUnsolicited = true;
    this->CheckForTaskStart();
}

void OContext::SetRestartIIN()
{
    this->staticIIN.SetBit(IINBit::DEVICE_RESTART);
}

IUpdateHandler& OContext::GetUpdateHandler()
{
    return this->database;
}

//// ----------------------------- function handlers -----------------------------

bool OContext::ProcessBroadcastRequest(const ParsedRequest& request)
{
    switch (request.header.function)
    {
    case (FunctionCode::WRITE):
        this->HandleWrite(request.objects);
        return true;
    case (FunctionCode::DIRECT_OPERATE_NR):
        this->HandleDirectOperate(request.objects, OperateType::DirectOperateNoAck, nullptr);
        return true;
    case (FunctionCode::IMMED_FREEZE_NR):
        this->HandleFreeze(request.objects);
        return true;
    case (FunctionCode::FREEZE_CLEAR_NR):
        this->HandleFreezeAndClear(request.objects);
        return true;
    case (FunctionCode::ASSIGN_CLASS):
    {
        if (this->application->SupportsAssignClass())
        {
            this->HandleAssignClass(request.objects);
            return true;
        }
        else
        {
            return false;
        }
    }
    case (FunctionCode::RECORD_CURRENT_TIME):
    {
        if (request.objects.is_not_empty())
        {
            this->HandleRecordCurrentTime();
            return true;
        }
        else
        {
            return false;
        }
    }
    case (FunctionCode::DISABLE_UNSOLICITED):
    {
        if (this->params.allowUnsolicited)
        {
            this->HandleDisableUnsolicited(request.objects, nullptr);
            return true;
        }
        else
        {
            return false;
        }
    }
    case (FunctionCode::ENABLE_UNSOLICITED):
    {
        if (this->params.allowUnsolicited)
        {
            this->HandleEnableUnsolicited(request.objects, nullptr);
            return true;
        }
        else
        {
            return false;
        }
    }
    default:
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring broadcast on function code: %s",
                         FunctionCodeSpec::to_string(request.header.function));
        return false;
    }
}

bool OContext::ProcessRequestNoAck(const ParsedRequest& request)
{
    switch (request.header.function)
    {
    case (FunctionCode::DIRECT_OPERATE_NR):
        this->HandleDirectOperate(request.objects, OperateType::DirectOperateNoAck,
                                  nullptr); // no object writer, this is a no ack code
        return true;
    case (FunctionCode::IMMED_FREEZE_NR):
        this->HandleFreeze(request.objects);
        return true;
    case (FunctionCode::FREEZE_CLEAR_NR):
        this->HandleFreezeAndClear(request.objects);
        return true;
    default:
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring NR function code: %s",
                         FunctionCodeSpec::to_human_string(request.header.function));
        return false;
    }
}

IINField OContext::HandleNonReadResponse(const APDUHeader& header, const ser4cpp::rseq_t& objects, HeaderWriter& writer)
{
    switch (header.function)
    {
    case (FunctionCode::WRITE):
        return this->HandleWrite(objects);
    case (FunctionCode::SELECT):
        return this->HandleSelect(objects, writer);
    case (FunctionCode::OPERATE):
        return this->HandleOperate(objects, writer);
    case (FunctionCode::DIRECT_OPERATE):
        return this->HandleDirectOperate(objects, OperateType::DirectOperate, &writer);
    case (FunctionCode::COLD_RESTART):
        return this->HandleRestart(objects, false, &writer);
    case (FunctionCode::WARM_RESTART):
        return this->HandleRestart(objects, true, &writer);
    case (FunctionCode::ASSIGN_CLASS):
        return this->HandleAssignClass(objects);
    case (FunctionCode::DELAY_MEASURE):
        return this->HandleDelayMeasure(objects, writer);
    case (FunctionCode::RECORD_CURRENT_TIME):
        return objects.is_empty() ? this->HandleRecordCurrentTime() : IINField(IINBit::PARAM_ERROR);
    case (FunctionCode::DISABLE_UNSOLICITED):
        return this->params.allowUnsolicited ? this->HandleDisableUnsolicited(objects, &writer)
                                             : IINField(IINBit::FUNC_NOT_SUPPORTED);
    case (FunctionCode::ENABLE_UNSOLICITED):
        return this->params.allowUnsolicited ? this->HandleEnableUnsolicited(objects, &writer)
                                             : IINField(IINBit::FUNC_NOT_SUPPORTED);
    case (FunctionCode::IMMED_FREEZE):
        return this->HandleFreeze(objects);
    case (FunctionCode::FREEZE_CLEAR):
        return this->HandleFreezeAndClear(objects);
    default:
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    }
}

ser4cpp::Pair<IINField, AppControlField> OContext::HandleRead(const ser4cpp::rseq_t& objects, HeaderWriter& writer)
{
    this->rspContext.Reset();
    this->eventBuffer.Unselect(); // always un-select any previously selected points when we start a new read request
    this->database.Unselect();

    ReadHandler handler(this->database, this->eventBuffer);
    auto result = APDUParser::Parse(objects, handler, &this->logger,
                                    ParserSettings::NoContents()); // don't expect range/count context on a READ
    if (result == ParseResult::OK)
    {
        auto control = this->rspContext.LoadResponse(writer);
        return ser4cpp::Pair<IINField, AppControlField>(handler.Errors(), control);
    }

    this->rspContext.Reset();
    return ser4cpp::Pair<IINField, AppControlField>(IINFromParseResult(result),
                                                    AppControlField(true, true, false, false));
}

IINField OContext::HandleWrite(const ser4cpp::rseq_t& objects)
{
    WriteHandler handler(*this->application, this->time, this->sol.seq.num, Timestamp(this->executor->get_time()),
                         &this->staticIIN);
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OContext::HandleDirectOperate(const ser4cpp::rseq_t& objects, OperateType opType, HeaderWriter* pWriter)
{
    // since we're echoing, make sure there's enough size before beginning
    if (pWriter && (objects.length() > pWriter->Remaining()))
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Igonring command request due to oversized payload size of %zu",
                         objects.length());
        return IINField(IINBit::PARAM_ERROR);
    }

    CommandActionAdapter adapter(*this->commandHandler, false, this->database, opType);
    CommandResponseHandler handler(this->params.maxControlsPerRequest, &adapter, pWriter);
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    this->shouldCheckForUnsolicited = true;
    return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
}

IINField OContext::HandleSelect(const ser4cpp::rseq_t& objects, HeaderWriter& writer)
{
    // since we're echoing, make sure there's enough size before beginning
    if (objects.length() > writer.Remaining())
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Igonring command request due to oversized payload size of %zu",
                         objects.length());
        return IINField(IINBit::PARAM_ERROR);
    }

    // the 'OperateType' is just ignored  since it's a select
    CommandActionAdapter adapter(*this->commandHandler, true, this->database, OperateType::DirectOperate);
    CommandResponseHandler handler(this->params.maxControlsPerRequest, &adapter, &writer);
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    if (result == ParseResult::OK)
    {
        if (handler.AllCommandsSuccessful())
        {
            this->control.Select(this->sol.seq.num, Timestamp(this->executor->get_time()), objects);
        }

        return handler.Errors();
    }

    return IINFromParseResult(result);
}

IINField OContext::HandleOperate(const ser4cpp::rseq_t& objects, HeaderWriter& writer)
{
    // since we're echoing, make sure there's enough size before beginning
    if (objects.length() > writer.Remaining())
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Igonring command request due to oversized payload size of %zu",
                         objects.length());
        return IINField(IINBit::PARAM_ERROR);
    }

    auto now = Timestamp(this->executor->get_time());
    auto result = this->control.ValidateSelection(this->sol.seq.num, now, this->params.selectTimeout, objects);

    if (result == CommandStatus::SUCCESS)
    {
        CommandActionAdapter adapter(*this->commandHandler, false, this->database, OperateType::SelectBeforeOperate);
        CommandResponseHandler handler(this->params.maxControlsPerRequest, &adapter, &writer);
        auto result = APDUParser::Parse(objects, handler, &this->logger);
        this->shouldCheckForUnsolicited = true;
        return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
    }
    else
    {
        this->control.Unselect();
    }

    return this->HandleCommandWithConstant(objects, writer, result);
}

IINField OContext::HandleDelayMeasure(const ser4cpp::rseq_t& objects, HeaderWriter& writer)
{
    if (objects.is_empty())
    {
        Group52Var2 value;
        value.time = 0; // respond with 0 time delay
        writer.WriteSingleValue<ser4cpp::UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);
        return IINField::Empty();
    }

    // there shouldn't be any trailing headers in delay measure request, no need to even parse
    return IINField(IINBit::PARAM_ERROR);
}

IINField OContext::HandleRecordCurrentTime()
{
    this->time.RecordCurrentTime(this->sol.seq.num, Timestamp(this->executor->get_time()));
    return IINField::Empty();
}

IINField OContext::HandleRestart(const ser4cpp::rseq_t& objects, bool isWarmRestart, HeaderWriter* pWriter)
{
    if (objects.is_not_empty())
        return IINField(IINBit::PARAM_ERROR);

    auto mode = isWarmRestart ? this->application->WarmRestartSupport() : this->application->ColdRestartSupport();

    switch (mode)
    {
    case (RestartMode::UNSUPPORTED):
        return IINField(IINBit::FUNC_NOT_SUPPORTED);
    case (RestartMode::SUPPORTED_DELAY_COARSE):
    {
        auto delay = isWarmRestart ? this->application->WarmRestart() : this->application->ColdRestart();
        if (pWriter)
        {
            Group52Var1 coarse;
            coarse.time = delay;
            pWriter->WriteSingleValue<ser4cpp::UInt8>(QualifierCode::UINT8_CNT, coarse);
        }
        return IINField::Empty();
    }
    default:
    {
        auto delay = isWarmRestart ? this->application->WarmRestart() : this->application->ColdRestart();
        if (pWriter)
        {
            Group52Var2 fine;
            fine.time = delay;
            pWriter->WriteSingleValue<ser4cpp::UInt8>(QualifierCode::UINT8_CNT, fine);
        }
        return IINField::Empty();
    }
    }
}

IINField OContext::HandleAssignClass(const ser4cpp::rseq_t& objects)
{
    if (this->application->SupportsAssignClass())
    {
        AssignClassHandler handler(*this->application, this->database);
        auto result = APDUParser::Parse(objects, handler, &this->logger, ParserSettings::NoContents());
        return (result == ParseResult::OK) ? handler.Errors() : IINFromParseResult(result);
    }

    return IINField(IINBit::FUNC_NOT_SUPPORTED);
}

IINField OContext::HandleDisableUnsolicited(const ser4cpp::rseq_t& objects, HeaderWriter* /*writer*/)
{
    ClassBasedRequestHandler handler;
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    if (result == ParseResult::OK)
    {
        this->params.unsolClassMask.Clear(handler.GetClassField());
        return handler.Errors();
    }

    return IINFromParseResult(result);
}

IINField OContext::HandleEnableUnsolicited(const ser4cpp::rseq_t& objects, HeaderWriter* /*writer*/)
{
    ClassBasedRequestHandler handler;
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    if (result == ParseResult::OK)
    {
        this->params.unsolClassMask.Set(handler.GetClassField());
        this->shouldCheckForUnsolicited = true;
        return handler.Errors();
    }

    return IINFromParseResult(result);
}

IINField OContext::HandleCommandWithConstant(const ser4cpp::rseq_t& objects, HeaderWriter& writer, CommandStatus status)
{
    ConstantCommandAction constant(status);
    CommandResponseHandler handler(this->params.maxControlsPerRequest, &constant, &writer);
    auto result = APDUParser::Parse(objects, handler, &this->logger);
    return IINFromParseResult(result);
}

IINField OContext::HandleFreeze(const ser4cpp::rseq_t& objects)
{
    FreezeRequestHandler handler(false, database);
    auto result = APDUParser::Parse(objects, handler, &this->logger, ParserSettings::NoContents());
    return IINFromParseResult(result);
}

IINField OContext::HandleFreezeAndClear(const ser4cpp::rseq_t& objects)
{
    FreezeRequestHandler handler(true, database);
    auto result = APDUParser::Parse(objects, handler, &this->logger, ParserSettings::NoContents());
    return IINFromParseResult(result);
}

} // namespace opendnp3
