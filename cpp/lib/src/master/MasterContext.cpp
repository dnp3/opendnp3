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
#include "MasterContext.h"

#include "app/APDUBuilders.h"
#include "app/APDULogging.h"
#include "app/parsing/APDUHeaderParser.h"
#include "gen/objects/Group12.h"
#include "gen/objects/Group41.h"
#include "logging/LogMacros.h"
#include "master/CommandTask.h"
#include "master/EmptyResponseTask.h"
#include "master/MeasurementHandler.h"
#include "master/RestartOperationTask.h"
#include "master/UserPollTask.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{
MContext::MContext(const Addresses& addresses,
                   const Logger& logger,
                   const std::shared_ptr<exe4cpp::IExecutor>& executor,
                   std::shared_ptr<ILowerLayer> lower,
                   const std::shared_ptr<ISOEHandler>& SOEHandler,
                   const std::shared_ptr<IMasterApplication>& application,
                   std::shared_ptr<IMasterScheduler> scheduler,
                   const MasterParams& params)
    : logger(logger),
      executor(executor),
      lower(std::move(lower)),
      addresses(addresses),
      params(params),
      SOEHandler(SOEHandler),
      application(application),
      scheduler(std::move(scheduler)),
      tasks(params, logger, *application, SOEHandler),
      txBuffer(params.maxTxFragSize),
      tstate(TaskState::IDLE)
{
}

bool MContext::OnLowerLayerUp()
{
    if (isOnline)
    {
        return false;
    }

    isOnline = true;

    tasks.Initialize(*this->scheduler, *this);

    this->application->OnOpen();

    return true;
}

bool MContext::OnLowerLayerDown()
{
    if (!isOnline)
    {
        return false;
    }

    tstate = TaskState::IDLE;
    responseTimer.cancel();
    solSeq = unsolSeq = 0;
    isOnline = isSending = false;
    activeTask.reset();

    this->scheduler->SetRunnerOffline(*this);
    this->application->OnClose();

    return true;
}

bool MContext::OnReceive(const Message& message)
{
    if (!this->isOnline)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::ERR, "Ignorning rx data while offline");
        return false;
    }

    if (message.addresses.destination != this->addresses.source)
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unknown destination address: %u", message.addresses.destination);
        return false;
    }

    if (message.addresses.source != this->addresses.destination)
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Unexpected message source: %u", message.addresses.source);
        return false;
    }

    const auto result = APDUHeaderParser::ParseResponse(message.payload, &this->logger);
    if (!result.success)
    {
        return true;
    }

    logging::LogHeader(this->logger, flags::APP_HEADER_RX, result.header);

    this->OnParsedHeader(message.payload, result.header, result.objects);

    return true;
}

bool MContext::OnTxReady()
{
    if (!this->isOnline || !this->isSending)
    {
        return false;
    }

    this->isSending = false;

    this->tstate = this->OnTransmitComplete();
    this->CheckConfirmTransmit();

    return true;
}

void MContext::OnResponseTimeout()
{
    if (isOnline)
    {
        this->tstate = this->OnResponseTimeoutEvent();
    }
}

void MContext::CompleteActiveTask()
{
    if (this->activeTask)
    {
        this->activeTask.reset();
        this->scheduler->CompleteCurrentFor(*this);
    }
}

void MContext::OnParsedHeader(const ser4cpp::rseq_t& /*apdu*/,
                              const APDUResponseHeader& header,
                              const ser4cpp::rseq_t& objects)
{
    // Note: this looks silly, but OnParsedHeader() is virtual and can be overriden to do SA
    this->ProcessAPDU(header, objects);
}

/// --- command handlers ----

void MContext::DirectOperate(CommandSet&& commands, const CommandResultCallbackT& callback, const TaskConfig& config)
{
    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;
    this->ScheduleAdhocTask(CommandTask::CreateDirectOperate(this->tasks.context, std::move(commands),
                                                             this->params.controlQualifierMode, *application, callback,
                                                             timeout, config, logger));
}

void MContext::SelectAndOperate(CommandSet&& commands, const CommandResultCallbackT& callback, const TaskConfig& config)
{
    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;
    this->ScheduleAdhocTask(CommandTask::CreateSelectAndOperate(this->tasks.context, std::move(commands),
                                                                this->params.controlQualifierMode, *application,
                                                                callback, timeout, config, logger));
}

void MContext::ProcessAPDU(const APDUResponseHeader& header, const ser4cpp::rseq_t& objects)
{
    switch (header.function)
    {
    case (FunctionCode::UNSOLICITED_RESPONSE):
        this->ProcessUnsolicitedResponse(header, objects);
        break;
    case (FunctionCode::RESPONSE):
        this->ProcessResponse(header, objects);
        break;
    default:
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Ignoring unsupported function code: %s",
                         FunctionCodeSpec::to_human_string(header.function));
        break;
    }
}

void MContext::ProcessIIN(const IINField& iin)
{
    if (iin.IsSet(IINBit::DEVICE_RESTART) && !this->params.ignoreRestartIIN)
    {
        this->tasks.OnRestartDetected();
        this->scheduler->Evaluate();
    }

    if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && this->params.integrityOnEventOverflowIIN)
    {
        if (this->tasks.DemandIntegrity())
            this->scheduler->Evaluate();
    }

    if (iin.IsSet(IINBit::NEED_TIME))
    {
        if (this->tasks.DemandTimeSync())
            this->scheduler->Evaluate();
    }

    if ((iin.IsSet(IINBit::CLASS1_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass1())
        || (iin.IsSet(IINBit::CLASS2_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass2())
        || (iin.IsSet(IINBit::CLASS3_EVENTS) && this->params.eventScanOnEventsAvailableClassMask.HasClass3()))
    {
        if (this->tasks.DemandEventScan())
            this->scheduler->Evaluate();
    }

    this->application->OnReceiveIIN(iin);
}

void MContext::ProcessUnsolicitedResponse(const APDUResponseHeader& header, const ser4cpp::rseq_t& objects)
{
    if (!header.control.UNS)
    {
        SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unsolicited response without UNS bit set");
        return;
    }

    auto result = MeasurementHandler::ProcessMeasurements(header.as_response_info(), objects, logger, SOEHandler.get());

    if ((result == ParseResult::OK) && header.control.CON)
    {
        this->QueueConfirm(APDUHeader::UnsolicitedConfirm(header.control.SEQ));
    }

    this->ProcessIIN(header.IIN);
}

void MContext::ProcessResponse(const APDUResponseHeader& header, const ser4cpp::rseq_t& objects)
{
    this->tstate = this->OnResponseEvent(header, objects);
    this->ProcessIIN(header.IIN); // TODO - should we process IIN bits for unexpected responses?
}

void MContext::QueueConfirm(const APDUHeader& header)
{
    this->confirmQueue.push_back(header);
    this->CheckConfirmTransmit();
}

bool MContext::CheckConfirmTransmit()
{
    if (this->isSending || this->confirmQueue.empty())
    {
        return false;
    }

    auto confirm = this->confirmQueue.front();
    APDUWrapper wrapper(this->txBuffer.as_wslice());
    wrapper.SetFunction(confirm.function);
    wrapper.SetControl(confirm.control);
    this->Transmit(wrapper.ToRSeq());
    this->confirmQueue.pop_front();
    return true;
}

void MContext::Transmit(const ser4cpp::rseq_t& data)
{
    logging::ParseAndLogRequestTx(this->logger, data);
    assert(!this->isSending);
    this->isSending = true;
    this->lower->BeginTransmit(Message(this->addresses, data));
}

void MContext::StartResponseTimer()
{
    auto timeout = [this]() { this->OnResponseTimeout(); };
    this->responseTimer = this->executor->start(this->params.responseTimeout.value, timeout);
}

std::shared_ptr<IMasterTask> MContext::AddScan(TimeDuration period,
                                               const HeaderBuilderT& builder,
                                               std::shared_ptr<ISOEHandler> soe_handler,
                                               TaskConfig config)
{
    auto task = std::make_shared<UserPollTask>(
        this->tasks.context, builder,
        TaskBehavior::ImmediatePeriodic(period, params.taskRetryPeriod, params.maxTaskRetryPeriod), true, *application,
        soe_handler, logger, config);
    this->ScheduleRecurringPollTask(task);
    return task;
}

std::shared_ptr<IMasterTask> MContext::AddClassScan(const ClassField& field,
                                                    TimeDuration period,
                                                    std::shared_ptr<ISOEHandler> soe_handler,
                                                    TaskConfig config)
{
    auto build = [field](HeaderWriter& writer) -> bool { return build::WriteClassHeaders(writer, field); };
    return this->AddScan(period, build, soe_handler, config);
}

std::shared_ptr<IMasterTask> MContext::AddAllObjectsScan(GroupVariationID gvId,
                                                         TimeDuration period,
                                                         std::shared_ptr<ISOEHandler> soe_handler,
                                                         TaskConfig config)
{
    auto build = [gvId](HeaderWriter& writer) -> bool { return writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS); };
    return this->AddScan(period, build, soe_handler, config);
}

std::shared_ptr<IMasterTask> MContext::AddRangeScan(GroupVariationID gvId,
                                                    uint16_t start,
                                                    uint16_t stop,
                                                    TimeDuration period,
                                                    std::shared_ptr<ISOEHandler> soe_handler,
                                                    TaskConfig config)
{
    auto build = [gvId, start, stop](HeaderWriter& writer) -> bool {
        return writer.WriteRangeHeader<ser4cpp::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);
    };
    return this->AddScan(period, build, soe_handler, config);
}

void MContext::Scan(const HeaderBuilderT& builder, std::shared_ptr<ISOEHandler> soe_handler, TaskConfig config)
{
    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;

    auto task
        = std::make_shared<UserPollTask>(this->tasks.context, builder, TaskBehavior::SingleExecutionNoRetry(timeout),
                                         false, *application, soe_handler, logger, config);

    this->ScheduleAdhocTask(task);
}

void MContext::ScanClasses(const ClassField& field, std::shared_ptr<ISOEHandler> soe_handler, TaskConfig config)
{
    auto configure = [field](HeaderWriter& writer) -> bool { return build::WriteClassHeaders(writer, field); };
    this->Scan(configure, soe_handler, config);
}

void MContext::ScanAllObjects(GroupVariationID gvId, std::shared_ptr<ISOEHandler> soe_handler, TaskConfig config)
{
    auto configure
        = [gvId](HeaderWriter& writer) -> bool { return writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS); };
    this->Scan(configure, soe_handler, config);
}

void MContext::ScanRange(
    GroupVariationID gvId, uint16_t start, uint16_t stop, std::shared_ptr<ISOEHandler> soe_handler, TaskConfig config)
{
    auto configure = [gvId, start, stop](HeaderWriter& writer) -> bool {
        return writer.WriteRangeHeader<ser4cpp::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);
    };
    this->Scan(configure, soe_handler, config);
}

void MContext::Write(const TimeAndInterval& value, uint16_t index, TaskConfig config)
{
    auto builder = [value, index](HeaderWriter& writer) -> bool {
        return writer.WriteSingleIndexedValue<ser4cpp::UInt16, TimeAndInterval>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                                Group50Var4::Inst(), value, index);
    };

    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;
    auto task = std::make_shared<EmptyResponseTask>(this->tasks.context, *this->application, "WRITE TimeAndInterval",
                                                    FunctionCode::WRITE, builder, timeout, this->logger, config);
    this->ScheduleAdhocTask(task);
}

void MContext::Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config)
{
    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;
    auto task = std::make_shared<RestartOperationTask>(this->tasks.context, *this->application, timeout, op, callback,
                                                       this->logger, config);
    this->ScheduleAdhocTask(task);
}

void MContext::PerformFunction(const std::string& name,
                               FunctionCode func,
                               const HeaderBuilderT& builder,
                               TaskConfig config)
{
    const auto timeout = Timestamp(this->executor->get_time()) + params.taskStartTimeout;
    auto task = std::make_shared<EmptyResponseTask>(this->tasks.context, *this->application, name, func, builder,
                                                    timeout, this->logger, config);
    this->ScheduleAdhocTask(task);
}

bool MContext::Run(const std::shared_ptr<IMasterTask>& task)
{
    if (this->activeTask || this->tstate != TaskState::IDLE)
        return false;

    this->tstate = TaskState::TASK_READY;
    this->activeTask = task;
    this->activeTask->OnStart();
    FORMAT_LOG_BLOCK(logger, flags::INFO, "Begining task: %s", this->activeTask->Name());

    if (!this->isSending)
    {
        this->tstate = this->ResumeActiveTask();
    }

    return true;
}

/// ------ private helpers ----------

void MContext::ScheduleRecurringPollTask(const std::shared_ptr<IMasterTask>& task)
{
    this->tasks.BindTask(task);

    if (this->isOnline)
    {
        this->scheduler->Add(task, *this);
    }
}

void MContext::ScheduleAdhocTask(const std::shared_ptr<IMasterTask>& task)
{
    if (this->isOnline)
    {
        this->scheduler->Add(task, *this);
    }
    else
    {
        // can't run this task since we're offline so fail it immediately
        task->OnLowerLayerClose(Timestamp(this->executor->get_time()));
    }
}

MContext::TaskState MContext::ResumeActiveTask()
{
    APDURequest request(this->txBuffer.as_wslice());

    /// try to build a requst for the task
    if (!this->activeTask->BuildRequest(request, this->solSeq))
    {
        activeTask->OnMessageFormatError(Timestamp(executor->get_time()));
        this->CompleteActiveTask();
        return TaskState::IDLE;
    }

    this->StartResponseTimer();
    auto apdu = request.ToRSeq();
    this->RecordLastRequest(apdu);
    this->Transmit(apdu);

    return TaskState::WAIT_FOR_RESPONSE;
}

//// --- State tables ---

MContext::TaskState MContext::OnTransmitComplete()
{
    switch (tstate)
    {
    case (TaskState::TASK_READY):
        return this->ResumeActiveTask();
    default:
        return tstate;
    }
}

MContext::TaskState MContext::OnResponseEvent(const APDUResponseHeader& header, const ser4cpp::rseq_t& objects)
{
    switch (tstate)
    {
    case (TaskState::WAIT_FOR_RESPONSE):
        return OnResponse_WaitForResponse(header, objects);
    default:
        FORMAT_LOG_BLOCK(logger, flags::WARN, "Not expecting a response, sequence: %u", header.control.SEQ);
        return tstate;
    }
}

MContext::TaskState MContext::OnResponseTimeoutEvent()
{
    switch (tstate)
    {
    case (TaskState::WAIT_FOR_RESPONSE):
        return OnResponseTimeout_WaitForResponse();
    default:
        SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unexpected response timeout");
        return tstate;
    }
}

//// --- State actions ----

MContext::TaskState MContext::StartTask_TaskReady()
{
    if (this->isSending)
    {
        return TaskState::TASK_READY;
    }

    return this->ResumeActiveTask();
}

MContext::TaskState MContext::OnResponse_WaitForResponse(const APDUResponseHeader& header,
                                                         const ser4cpp::rseq_t& objects)
{
    if (header.control.SEQ != this->solSeq)
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Response with bad sequence: %u", header.control.SEQ);
        return TaskState::WAIT_FOR_RESPONSE;
    }

    this->responseTimer.cancel();

    this->solSeq.Increment();

    auto now = Timestamp(this->executor->get_time());

    auto result = this->activeTask->OnResponse(header, objects, now);

    if (header.control.CON)
    {
        this->QueueConfirm(APDUHeader::SolicitedConfirm(header.control.SEQ));
    }

    switch (result)
    {
    case (IMasterTask::ResponseResult::OK_CONTINUE):
        this->StartResponseTimer();
        return TaskState::WAIT_FOR_RESPONSE;
    case (IMasterTask::ResponseResult::OK_REPEAT):
        return StartTask_TaskReady();
    default:
        // task completed or failed, either way go back to idle
        this->CompleteActiveTask();
        return TaskState::IDLE;
    }
}

MContext::TaskState MContext::OnResponseTimeout_WaitForResponse()
{
    SIMPLE_LOG_BLOCK(logger, flags::WARN, "Timeout waiting for response");

    auto now = Timestamp(this->executor->get_time());
    this->activeTask->OnResponseTimeout(now);
    this->solSeq.Increment();
    this->CompleteActiveTask();
    return TaskState::IDLE;
}
} // namespace opendnp3
