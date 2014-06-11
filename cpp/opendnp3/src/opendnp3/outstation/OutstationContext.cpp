/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "OutstationContext.h"

#include "opendnp3/StaticSizeConfiguration.h"
#include "opendnp3/LogLevels.h"

#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/APDUParser.h"
#include "opendnp3/app/APDUHeaderParser.h"

#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/WriteHandler.h"
#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"
#include "opendnp3/outstation/ConstantCommandAction.h"
#include "opendnp3/outstation/EventWriter.h"

#include "opendnp3/outstation/ClassBasedRequestHandler.h"

#include <openpal/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

OutstationContext::OutstationContext(
		const OutstationConfig& config,
		IExecutor& executor,
		LogRoot& root,
		ILowerLayer& lower,
		ICommandHandler& commandHandler,
		ITimeWriteHandler& timeWriteHandler,
		Database& database,
		const EventBufferFacade& buffers) :
	
	params(config.params),
	eventConfig(config.defaultEventResponses),
	logger(root.GetLogger()),	
	pExecutor(&executor),	
	pCommandHandler(&commandHandler),
	pTimeWriteHandler(&timeWriteHandler),
	pDatabase(&database),
	eventBuffer(buffers),
	isOnline(false),
	txState(TxState::IDLE),
	pSolicitedState(&OutstationSolicitedStateIdle::Inst()),
	pConfirmTimer(nullptr),
	pUnsolTimer(nullptr),
	unsolPackTimerExpired(false),
	rxFragCount(0),		
	operateExpectedSeq(0),
	operateExpectedFragCount(0),
	solSeqN(0),
	unsolSeqN(0),
	expectedSolConfirmSeq(0),
	expectedUnsolConfirmSeq(0),
	completedNullUnsol(false),	
	rspContext(&database, &eventBuffer, StaticResponseTypes(config.defaultStaticResponses)),
	pLower(&lower)
{
	pDatabase->SetEventBuffer(eventBuffer);
	staticIIN.Set(IINBit::DEVICE_RESTART);

	if (params.maxTxFragSize < sizes::MIN_APDU_SIZE)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, 
			"setting maxTxFragSize of %u to minimum of %u", 
			static_cast<unsigned int>(params.maxTxFragSize), 
			static_cast<unsigned int>(sizes::MIN_APDU_SIZE));

		params.maxTxFragSize = sizes::MIN_APDU_SIZE;
	}

	if (params.allowUnsolicited)
	{
		// this will cause us to start going through the NULL unsolicited sequence				
		this->OnEnterIdleState();
	}
		
	auto notify = [this]() { this->OnNewEvents(); };
	auto post = [notify, this] { pExecutor->PostLambda(notify); };
	database.SetEventHandler(Bind(post));
}

IINField OutstationContext::GetDynamicIIN()
{
	IINField ret;
	auto count = eventBuffer.UnselectedEvents();
	if (count.numClass1)
	{
		ret.Set(IINBit::CLASS1_EVENTS);
	}
	if (count.numClass2)
	{
		ret.Set(IINBit::CLASS2_EVENTS);
	}
	if (count.numClass3)
	{
		ret.Set(IINBit::CLASS3_EVENTS);
	}
	if (eventBuffer.IsOverflown())
	{
		ret.Set(IINBit::EVENT_BUFFER_OVERFLOW);
	}
	return ret;
}

IINField OutstationContext::GetResponseIIN()
{
	return this->staticIIN | GetDynamicIIN();
}

APDUResponse OutstationContext::StartNewResponse()
{	
	return APDUResponse(txBuffer.GetWriteBuffer(params.maxTxFragSize));
}

void OutstationContext::ConfigureUnsolHeader(APDUResponse& unsol)
{	
	build::NullUnsolicited(unsol, this->unsolSeqN, this->GetResponseIIN());	
}

void OutstationContext::SetOnline()
{
	isOnline = true;
}

void OutstationContext::SetOffline()
{
	isOnline = false;
	unsolPackTimerExpired = false;
	txState = TxState::IDLE;
	pSolicitedState = &OutstationSolicitedStateIdle::Inst();
	lastValidRequest.Clear();
	deferredRequest.Clear();
	eventBuffer.Reset();
	rspContext.Reset();
	CancelConfirmTimer();
	CancelUnsolTimer();
}

bool OutstationContext::IsOperateSequenceValid()
{	
	return (rxFragCount == operateExpectedFragCount) && (solSeqN == operateExpectedSeq);	
}

bool OutstationContext::IsIdle()
{
	return isOnline && (txState == TxState::IDLE) && pSolicitedState == &OutstationSolicitedStateIdle::Inst();
}

bool OutstationContext::CancelConfirmTimer()
{
	return CancelTimer(pConfirmTimer);
}

bool OutstationContext::CancelUnsolTimer()
{
	return CancelTimer(pUnsolTimer);
}

bool OutstationContext::CancelTimer(openpal::ITimer*& pTimer)
{
	if (pTimer)
	{
		pTimer->Cancel();
		pTimer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

void OutstationContext::OnReceiveAPDU(const openpal::ReadOnlyBuffer& apdu)
{
	++rxFragCount;

	APDUHeader header;	
	if (APDUHeaderParser::ParseRequest(apdu, header, &logger))
	{
		// outstations should only process single fragment messages
		if ((header.control.FIR && header.control.FIN) && !header.control.CON)
		{
			if (header.control.UNS)
			{
				SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unsol message");
			}
			else
			{
				if (header.function == FunctionCode::CONFIRM)
				{
					pSolicitedState->OnConfirm(this, header);
				}
				else
				{
					this->OnReceiveSolRequest(header, apdu);
				}
			}
		}
		else
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN,
				"Ignoring fragment with unexpected control field - FIR: %u FIN: %u CON: %u",
				header.control.FIN,
				header.control.FIN,
				header.control.CON);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "ignoring malformed request header");
	}
}

void OutstationContext::OnSendResult(bool isSuccess)
{
	auto state = txState;
	txState = TxState::IDLE;
	if (state == TxState::SOLICITED)
	{		
		pSolicitedState->OnSendResult(this, isSuccess);
	}
}

void OutstationContext::OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& apdu)
{
	// analyze this request to see how it compares to the last request
	auto firstRequest = lastValidRequest.IsEmpty();
	auto equality = APDURequest::Compare(apdu, lastValidRequest);
	auto dest = rxBuffer.GetWriteBuffer();
	this->lastValidRequest = apdu.CopyTo(dest);
	auto objects = apdu.Skip(APDU_HEADER_SIZE);

	if (firstRequest)
	{			
		this->solSeqN = header.control.SEQ;
		this->pSolicitedState->OnNewRequest(this, header, objects, APDUEquality::NONE);
	}
	else
	{		
		if (this->solSeqN == header.control.SEQ)
		{
			if (equality == APDUEquality::FULL_EQUALITY)
			{
				this->pSolicitedState->OnRepeatRequest(this, header, objects);
			}
			else // new operation with same SEQ
			{
				this->pSolicitedState->OnNewRequest(this, header, objects, equality);
			}
		}
		else  // completely new sequence #
		{
			this->solSeqN = header.control.SEQ;
			this->pSolicitedState->OnNewRequest(this, header, objects, equality);
		}
	}

}

void OutstationContext::RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, bool objectsEqualToLastRequest)
{
	auto response = StartNewResponse();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
	auto iin = this->BuildNonReadResponse(header, objects, writer, objectsEqualToLastRequest);
	response.SetIIN(iin | this->GetResponseIIN());		
	this->BeginResponseTx(response.ToReadOnly());
}

void OutstationContext::BeginResponseTx(const ReadOnlyBuffer& response)
{	
	txState = TxState::SOLICITED;
	lastResponse = response;
	pLower->BeginTransmit(response);	
}

void OutstationContext::BeginUnsolTx(const ReadOnlyBuffer& response)
{
	txState = TxState::UNSOLICITED;
	this->expectedUnsolConfirmSeq = unsolSeqN;
	this->unsolSeqN = AppControlField::NextSeq(unsolSeqN);
	pLower->BeginTransmit(response);
}

IINField OutstationContext::BuildNonReadResponse(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, bool objectsEqualToLastRequest)
{
	switch (header.function)
	{				
		case(FunctionCode::WRITE) :
			return HandleWrite(objects);
		case(FunctionCode::SELECT) :
			return HandleSelect(objects, writer);
		case(FunctionCode::OPERATE) :
			return HandleOperate(objects, writer, objectsEqualToLastRequest);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(objects, writer);
		case(FunctionCode::DELAY_MEASURE) :
			return HandleDelayMeasure(objects, writer);
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return HandleDisableUnsolicited(objects, writer);
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return HandleEnableUnsolicited(objects, writer);
		default:
			return	IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

void OutstationContext::ContinueMultiFragResponse(uint8_t seq)
{
	auto response = this->StartNewResponse();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);

	openpal::Transaction tx(this->pDatabase);	
	auto control = this->rspContext.LoadSolicited(writer, eventConfig);
	control.SEQ = seq;
	response.SetControl(control);
	response.SetIIN(this->staticIIN | this->GetDynamicIIN());
	if (response.GetControl().CON)
	{
		expectedSolConfirmSeq = seq;
		this->pSolicitedState = &OutstationStateSolicitedConfirmWait::Inst();
	}
	this->BeginResponseTx(response.ToReadOnly());
}

void OutstationContext::OnEnterIdleState()
{
	// post these calls so the stack can unwind
	auto lambda = [this]() { this->PerformTaskFromIdleState(); };
	pExecutor->PostLambda(lambda);
}

void OutstationContext::PerformTaskFromIdleState()
{	
	this->CheckDeferredRequest();

	this->CheckForUnsolicited();	
}

void OutstationContext::CheckDeferredRequest()
{
	DeferredRequest request;
	if (this->IsIdle() && deferredRequest.Pop(request))
	{
		if (request.lastEquality == APDUEquality::FULL_EQUALITY) // it was a repeat
		{
			this->pSolicitedState->OnRepeatRequest(this, request.header, lastValidRequest.Skip(APDU_HEADER_SIZE));
		}
		else
		{
			this->pSolicitedState->OnNewRequest(this, request.header, lastValidRequest.Skip(APDU_HEADER_SIZE), request.lastEquality);
		}		
	}	
}

void OutstationContext::OnNewEvents()
{
	this->CheckForUnsolicited();	
}

void OutstationContext::CheckForUnsolicited()
{
	/*
	if (this->IsIdle() && params.allowUnsolicited && (pUnsolTimer == nullptr))
	{
		if (completedNullUnsol)
		{
			auto criteria = SelectionCriteria::ForomUnsolMask(params.unsolClassMask);
			if (criteria.HasSelection())
			{
				auto unsol = this->StartNewResponse();
				auto initialSize = unsol.Size();
						
				{
					// even though we're not loading static data, we need to lock 
					// the database since it updates the event buffer					
					openpal::Transaction tx(pDatabase);
					auto iterator = eventBuffer.SelectEvents(criteria);
					auto writer = unsol.GetWriter();
					EventWriter::WriteEventHeaders(writer, iterator, eventConfig);
				}
			
				if (unsol.Size() > initialSize) // were any events written?
				{
					this->ConfigureUnsolHeader(unsol);
					this->pSolicitedState = &OutstationStateUnsolConfirmWait::Inst();
					this->BeginUnsolTx(unsol.ToReadOnly());
				}
				else
				{ 
					eventBuffer.Reset();
				}
			}
		}
		else
		{
			// send a NULL unsolcited message			
			pState = &OutstationStateUnsolConfirmWait::Inst();
			auto unsol = this->StartNewResponse();
			this->ConfigureUnsolHeader(unsol);
			this->BeginUnsolTx(unsol.ToReadOnly());
		}
	}
	*/
}

bool OutstationContext::StartConfirmTimer()
{
	if (pConfirmTimer)
	{	
		return false;		
	}
	else
	{
		auto timeout = [this]() { this->OnSolConfirmTimeout(); };
		pConfirmTimer = pExecutor->Start(params.solConfirmTimeout, Bind(timeout));
		return true;
	}
}

bool OutstationContext::StartUnsolRetryTimer()
{
	if (pUnsolTimer)
	{
		return false;		
	}
	else
	{
		auto timeout = [this]() { this->OnUnsolRetryTimeout(); };
		pUnsolTimer = pExecutor->Start(params.unsolRetryTimeout, Bind(timeout));
		return true;
	}
}

void OutstationContext::OnSolConfirmTimeout()
{
	this->pSolicitedState->OnConfirmTimeout(this);
}

void OutstationContext::OnUnsolRetryTimeout()
{
	pUnsolTimer = nullptr;
	this->CheckForUnsolicited();
}

APDUResponseHeader OutstationContext::HandleRead(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	rspContext.Reset();
	ReadHandler handler(logger, rspContext);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &logger, APDUParser::Context(false)); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{
		// Do a transaction on the database (lock) for multi-threaded environments
		// if the request contained static variations, we double buffer (copy) the entire static database.
		// this ensures that multi-fragmented responses see a consistent snapshot of the state
		openpal::Transaction tx(pDatabase);
		pDatabase->DoubleBuffer();
		auto control = rspContext.LoadSolicited(writer, eventConfig);		
		return APDUResponseHeader(control, handler.Errors());
	}
	else
	{
		rspContext.Reset();
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField OutstationContext::HandleWrite(const openpal::ReadOnlyBuffer& objects)
{
	WriteHandler handler(logger, pTimeWriteHandler, &staticIIN);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
	if (result == APDUParser::Result::OK)
	{
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleDirectOperate(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(pCommandHandler, false);
		CommandResponseHandler handler(logger, params.maxControlsPerRequest, &adapter, writer);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleSelect(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(pCommandHandler, true);
		CommandResponseHandler handler(logger, params.maxControlsPerRequest, &adapter, writer);
		auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
		if (result == APDUParser::Result::OK)
		{
			if (handler.AllCommandsSuccessful())
			{				
				operateExpectedFragCount = rxFragCount + 1;
				operateExpectedSeq = AppControlField::NextSeq(solSeqN);
				selectTime = pExecutor->GetTime();
				return IINField::Empty;
			}
			else
			{
				return IINField::Empty;
			}
		}
		else
		{
			return IINFromParseResult(result);
		}
	}
}

IINField OutstationContext::HandleOperate(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, bool objectsEqualToLastRequest)
{
	// since we're echoing, make sure there's enough size before beginning
	if (objects.Size() > writer.Remaining())
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Igonring command request due to payload size of %i", objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		if (this->IsOperateSequenceValid())
		{
			auto elapsed = pExecutor->GetTime().milliseconds - selectTime.milliseconds;
			if (elapsed < params.selectTimeout.GetMilliseconds())
			{
				if (objectsEqualToLastRequest)
				{					
					CommandActionAdapter adapter(pCommandHandler, false);
					CommandResponseHandler handler(logger, params.maxControlsPerRequest, &adapter, writer);
					auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
					return IINFromParseResult(result);					
				}
				else
				{
					return HandleCommandWithConstant(objects, writer, CommandStatus::NO_SELECT);
				}
			}
			else
			{
				return HandleCommandWithConstant(objects, writer, CommandStatus::TIMEOUT);

			}
		}
		else
		{
			return HandleCommandWithConstant(objects, writer, CommandStatus::NO_SELECT);
		}
	}
}

IINField OutstationContext::HandleDelayMeasure(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	if (objects.IsEmpty())
	{		
		Group52Var2 value = { 0 }; 	// respond with 0 time delay
		writer.WriteSingleValue<UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);
		return IINField::Empty;
	}
	else
	{
		// there shouldn't be any trailing headers in delay measure request, no need to even parse
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField OutstationContext::HandleDisableUnsolicited(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	ClassBasedRequestHandler handler(logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
	if (result == APDUParser::Result::OK)
	{
		params.unsolClassMask &= ~handler.GetClassMask();
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleEnableUnsolicited(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer)
{
	ClassBasedRequestHandler handler(logger);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
	if (result == APDUParser::Result::OK)
	{
		params.unsolClassMask |= handler.GetClassMask();
		return handler.Errors();
	}
	else
	{
		return IINFromParseResult(result);
	}
}

IINField OutstationContext::HandleCommandWithConstant(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, CommandStatus status)
{
	ConstantCommandAction constant(status);
	CommandResponseHandler handler(logger, params.maxControlsPerRequest, &constant, writer);
	auto result = APDUParser::ParseTwoPass(objects, &handler, &logger);
	return IINFromParseResult(result);
}


}

