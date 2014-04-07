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
#include "AppLayer.h"

#include <openpal/LogMacros.h>
#include <openpal/IExecutor.h>
#include <openpal/LogRoot.h>

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/LogLevels.h"

#include "FunctionCodeHelpers.h"

#include <assert.h>

using namespace std;

namespace opendnp3
{

AppLayer::AppLayer(openpal::LogRoot& root, openpal::IExecutor* pExecutor_, const AppConfig& config) :
	logger(root.GetLogger(sources::APP_LAYER)),
	isOnline(false),
	isSending(false),
	isConfirmSending(false),
	isMaster(config.IsMaster),
	pUser(nullptr),
	pTransportLayer(nullptr),
	solicited(logger, this, pExecutor_, config.RspTimeout),
	unsolicited(logger, this, pExecutor_, config.RspTimeout),
	numRetry(config.NumRetry),
	confirmAPDU(openpal::WriteBuffer(confirmBuffer, 2))
{
	confirmAPDU.SetFunction(FunctionCode::CONFIRM);
}

void AppLayer::SetUser(IAppUser* apUser)
{
	assert(pUser == nullptr);
	assert(apUser != nullptr);
	pUser = apUser;
}

void AppLayer::SetTransportLayer(openpal::ILowerLayer* pTransportLayer_)
{
	assert(pTransportLayer == nullptr);
	assert(pTransportLayer_ != nullptr);
	pTransportLayer = pTransportLayer_;
}

////////////////////
// IAppLayer
////////////////////

void AppLayer::SendResponse(APDUResponse& apdu)
{
	solicited.Send(apdu, this->GetRetries(FunctionCode::RESPONSE));
}

void AppLayer::SendUnsolicited(APDUResponse& apdu)
{
	unsolicited.Send(apdu, this->GetRetries(FunctionCode::UNSOLICITED_RESPONSE));
}

void AppLayer::SendRequest(APDURequest& apdu)
{
	solicited.Send(apdu, this->GetRetries(apdu.GetFunction()));
}

void AppLayer::CancelResponse()
{
	solicited.Cancel();
}

////////////////////
// External events
////////////////////

void AppLayer::OnReceive(const ReadOnlyBuffer& apdu)
{
	if(isOnline)
	{
		if (this->isMaster)
		{
			APDUResponseRecord record;
			auto result = APDUHeaderParser::ParseResponse(apdu, record);
			if (result == APDUHeaderParser::Result::OK)
			{
				switch (record.function)
				{
				case(FunctionCode::CONFIRM) :
					this->OnConfirm(record.control, record.objects.Size());
					break;
				case(FunctionCode::RESPONSE) :
					this->OnResponse(record);
					break;
				case(FunctionCode::UNSOLICITED_RESPONSE) :
					this->OnUnsolResponse(record);
					break;
				default:
					FORMAT_LOG_BLOCK(logger, flags::WARN, "Unexpected function code for master: %s", FunctionCodeToString(record.function));
					break;
				}
			}
			else LogParseError(result, true);
		}
		else
		{
			APDURecord record;
			auto result = APDUHeaderParser::ParseRequest(apdu, record);
			if (result == APDUHeaderParser::Result::OK)
			{
				switch (record.function)
				{
				case(FunctionCode::CONFIRM) :
					this->OnConfirm(record.control, record.objects.Size());
					break;
				default: //otherwise, assume it's a request
					this->OnRequest(record);
					break;
				}
			}
			else LogParseError(result, false);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not up");
	}
}

void AppLayer::LogParseError(APDUHeaderParser::Result error, bool aIsResponse)
{
	switch(error)
	{
	case(APDUHeaderParser::Result::NOT_ENOUGH_DATA_FOR_HEADER):
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, ALERR_INSUFFICIENT_DATA_FOR_FRAG, "Not enough data for header while parsing %s", (aIsResponse ? "respose" : "request"));
		break;
	default:
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Unspecified parse error");
		break;
	}
}

void AppLayer::OnLowerLayerUp()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is already online");
	}
	else
	{
		isOnline = true;
		pUser->OnLowerLayerUp();
	}
}

void AppLayer::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;

		//reset both the channels
		solicited.Reset();
		unsolicited.Reset();

		//reset the transmitter state
		sendQueue.Clear();
		isSending = false;

		//notify the user
		pUser->OnLowerLayerDown();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not online");
	}
}

void AppLayer::OnSendResult(bool isSuccess)
{
	if(isSending)
	{
		assert(sendQueue.Size() > 0);
		isSending = false;

		auto function = sendQueue.Pop().GetFunction();
		

		if (function == FunctionCode::CONFIRM)
		{
			assert(isConfirmSending);
			isConfirmSending = false;
		}
		else
		{
			if (isSuccess)
			{
				if (function == FunctionCode::UNSOLICITED_RESPONSE)
				{
					unsolicited.OnSendSuccess();
				}
				else
				{
					solicited.OnSendSuccess();
				}
			}
			else
			{
				if (function == FunctionCode::UNSOLICITED_RESPONSE)
				{
					unsolicited.OnSendFailure();
				}
				else
				{
					solicited.OnSendFailure();
				}
			}
		}

		this->CheckForSend();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Layer is not sending");
	}
}

////////////////////
// Internal Events
////////////////////

void AppLayer::OnResponse(const APDUResponseRecord& record)
{
	if(record.control.UNS)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, ALERR_BAD_UNSOL_BIT, "Bad unsol bit");
	}
	else
	{
		// If we get a response that requests confirmation, we shouldn't confirm
		// if we're not going to handle the data. This is usually indicative of an
		// early timeout. It will show up in the logs as a response without context.
		if(record.control.CON && solicited.AcceptsResponse())
		{
			this->QueueConfirm(false, record.control.SEQ);
		}

		solicited.OnResponse(record);
	}
}

void AppLayer::OnUnsolResponse(const APDUResponseRecord& record)
{
	if(!record.control.UNS)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, ALERR_BAD_UNSOL_BIT, "Unsolicited response code with uns bit not set");
	}
	else
	{

		if (record.control.CON)
		{
			this->QueueConfirm(true, record.control.SEQ);
		}

		unsolicited.OnUnsol(record);
	}


}

void AppLayer::OnConfirm(const AppControlField& aControl, uint32_t aDataSize)
{
	if(aDataSize > 0)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Unexpected payload in confirm of size: %u", static_cast<unsigned int>(aDataSize));
	}
	else
	{
		if(aControl.UNS) // which channel?
		{
			if(isMaster)
			{
				SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::ERR, ALERR_UNEXPECTED_CONFIRM, "Unexpcted confirm for master")
			}
			else
			{
				unsolicited.OnConfirm(aControl.SEQ);
			}
		}
		else
		{
			solicited.OnConfirm(aControl.SEQ);
		}
	}
}

void AppLayer::OnRequest(const APDURecord& record)
{
	if(record.control.UNS)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, ALERR_BAD_UNSOL_BIT, "Received request with UNS bit");
	}
	else
	{
		if (record.control.IsFirAndFin())
		{
			solicited.OnRequest(record);
		}
		else
		{
			SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, ALERR_MULTI_FRAGEMENT_REQUEST, "Received non FIR/FIN request");
		}
	}
}

////////////////////
// Helpers
////////////////////

void AppLayer::QueueConfirm(bool aUnsol, int aSeq)
{
	if(isConfirmSending)
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::WARN, aUnsol ? ALERR_UNSOL_FLOOD : ALERR_SOL_FLOOD, "Confirm request flood, ignoring confirm");
	}
	else
	{
		isConfirmSending = true;
		AppControlField acf(true, true, false, aUnsol, aSeq);
		confirmAPDU.SetControl(acf);
		this->QueueFrame(confirmAPDU);
	}
}


void AppLayer::QueueFrame(const APDUWrapper& apdu)
{
	assert(!sendQueue.IsFull());
	sendQueue.Enqueue(apdu);
	this->CheckForSend();
}

void AppLayer::CheckForSend()
{
	if(!isSending && sendQueue.Size() > 0)
	{
		isSending = true;		
		pTransportLayer->Send(sendQueue.Peek().ToReadOnly());
	}
}

uint32_t AppLayer::GetRetries(FunctionCode aCode)
{
	switch(aCode)
	{
	case(FunctionCode::DIRECT_OPERATE):
	case(FunctionCode::DIRECT_OPERATE_NO_ACK):
	case(FunctionCode::RESPONSE):
	case(FunctionCode::WRITE): // b/c these can contain time objects which are sensitive to retries
		return 0;
	default:
		return numRetry; //use the configured
	}
}

} //end ns
