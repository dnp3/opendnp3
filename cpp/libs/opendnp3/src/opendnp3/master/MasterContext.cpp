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

#include "MasterContext.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/APDUWrapper.h"
#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/APDULogging.h"
#include "opendnp3/master/MeasurementHandler.h"

#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group41.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

MasterContext::MasterContext(
	IExecutor& executor,
	LogRoot& root,
	ILowerLayer& lower,
	ISOEHandler& SOEHandler,	
	IMasterApplication& application,
	const MasterParams& params,
	ITaskLock& taskLock
	) :

	mstate(executor, root, lower, SOEHandler, application, *this, params, taskLock),
	commandProcessor(mstate)
{
	
}

bool MasterContext::OnLayerUp()
{
	return mstate.GoOnline();
}

bool MasterContext::OnLayerDown()
{
	return mstate.GoOffline();
}

void MasterContext::OnPendingTask()
{
	MasterActions::PostCheckForTask(mstate);
}

void MasterContext::OnSendResult(bool isSucccess)
{
	if (mstate.isOnline && mstate.isSending)
	{
		mstate.isSending = false;

		MasterActions::CheckConfirmTransmit(mstate);
		MasterActions::CheckForTask(mstate);
	}
}

void MasterContext::OnReceive(const ReadBufferView& apdu)
{
	if (mstate.isOnline)
	{
		APDUResponseHeader header;		
		if (APDUHeaderParser::ParseResponse(apdu, header, &mstate.logger))
		{
			FORMAT_LOG_BLOCK(mstate.logger, flags::APP_HEADER_RX,
				"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
				header.control.FIR,
				header.control.FIN,
				header.control.CON,
				header.control.UNS,
				header.control.SEQ,
				FunctionCodeToString(header.function),
				header.IIN.LSB,
				header.IIN.MSB);

			if (header.control.UNS)
			{
				if (header.function == FunctionCode::UNSOLICITED_RESPONSE)
				{
					this->OnUnsolicitedResponse(header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported function with UNS bit set: %s", FunctionCodeToString(header.function));
				}
			}
			else
			{
				if (header.function == FunctionCode::RESPONSE)
				{
					this->mstate.pState = mstate.pState->OnResponse(mstate, header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
					this->OnReceiveIIN(header.IIN);
				}
				else
				{
					FORMAT_LOG_BLOCK(mstate.logger, flags::WARN, "Ignoring unsupported solicited function code: %s", FunctionCodeToString(header.function));
				}
			}						
		}
	}
}


void MasterContext::OnUnsolicitedResponse(const APDUResponseHeader& header, const ReadBufferView& objects)
{
	if (header.control.UNS)
	{		
		auto result = MeasurementHandler::ProcessMeasurements(objects, mstate.logger, mstate.pSOEHandler);

		if ((result == ParseResult::OK) && header.control.CON)
		{			
			MasterActions::QueueConfirm(mstate, APDUHeader::UnsolicitedConfirm(header.control.SEQ));			
		}
	}	
}

void MasterContext::OnReceiveIIN(const IINField& iin)
{
	mstate.pApplication->OnReceiveIIN(iin);
	
	if (iin.IsSet(IINBit::DEVICE_RESTART))
	{
		mstate.tasks.clearRestart.Demand();
		mstate.tasks.assignClass.Demand();
		mstate.tasks.startupIntegrity.Demand();
		mstate.tasks.enableUnsol.Demand();
	}

	if (iin.IsSet(IINBit::EVENT_BUFFER_OVERFLOW) && mstate.params.integrityOnEventOverflowIIN)
	{		
		mstate.tasks.startupIntegrity.Demand();
	}

	if (iin.IsSet(IINBit::NEED_TIME))
	{
		mstate.tasks.timeSync.Demand();
	}	

	if ((iin.IsSet(IINBit::CLASS1_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass1()) ||
		(iin.IsSet(IINBit::CLASS2_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass2()) ||
		(iin.IsSet(IINBit::CLASS3_EVENTS) && mstate.params.eventScanOnEventsAvailableClassMask.HasClass3()))
	{
		mstate.tasks.eventScan.Demand();
	}
}

}
