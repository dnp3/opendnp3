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

#include <openpal/LoggableMacros.h>
#include <openpal/IExecutor.h>


#include "FunctionCodeHelpers.h"

#include <assert.h>

using namespace std;

namespace opendnp3
{

AppLayer::AppLayer(Logger aLogger, openpal::IExecutor* apExecutor, AppConfig aAppCfg) :
	Loggable(aLogger),
	IUpperLayer(aLogger),
	mSending(false),
	mConfirmSending(false),
	mIsMaster(aAppCfg.IsMaster),
	mpUser(nullptr),
	mSolicited(aLogger.GetSubLogger("sol"), this, apExecutor, aAppCfg.RspTimeout),
	mUnsolicited(aLogger.GetSubLogger("unsol"), this, apExecutor, aAppCfg.RspTimeout),
	mNumRetry(aAppCfg.NumRetry),
	confirmAPDU(openpal::WriteBuffer(confirmBuffer, 2))
{
	confirmAPDU.SetFunction(FunctionCode::CONFIRM);
}

void AppLayer::SetUser(IAppUser* apUser)
{
	assert(mpUser == nullptr);
	assert(apUser != nullptr);
	mpUser = apUser;
}

////////////////////
// IAppLayer
////////////////////

void AppLayer::SendResponse(APDUResponse& apdu)
{
	mSolicited.Send(apdu, this->GetRetries(FunctionCode::RESPONSE));
}

void AppLayer::SendUnsolicited(APDUResponse& apdu)
{
	mUnsolicited.Send(apdu, this->GetRetries(FunctionCode::UNSOLICITED_RESPONSE));
}

void AppLayer::SendRequest(APDURequest& apdu)
{
	mSolicited.Send(apdu, this->GetRetries(apdu.GetFunction()));
}

void AppLayer::CancelResponse()
{
	mSolicited.Cancel();
}

////////////////////
// External events
////////////////////

void AppLayer::_OnReceive(const ReadOnlyBuffer& aBuffer)
{
	if(this->IsLowerLayerUp())
	{
		if (this->mIsMaster)
		{
			APDUResponseRecord record;
			auto result = APDUHeaderParser::ParseResponse(aBuffer, record);
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
					LOG_BLOCK(LogLevel::Warning, "Unexpected function code for master: " << FunctionCodeToString(record.function));
					break;
				}
			}
			else LogParseError(result, true);
		}
		else
		{
			APDURecord record;
			auto result = APDUHeaderParser::ParseRequest(aBuffer, record);
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
		LOG_BLOCK(LogLevel::Error, "Layer is not up");
	}
}

void AppLayer::LogParseError(APDUHeaderParser::Result error, bool aIsResponse)
{
	switch(error)
	{
	case(APDUHeaderParser::Result::NOT_ENOUGH_DATA_FOR_HEADER):
		ERROR_BLOCK(LogLevel::Error, "Not enough data for header while parsing " << (aIsResponse ? "respose" : "request"), ALERR_INSUFFICIENT_DATA_FOR_FRAG);
		break;
	default:
		LOG_BLOCK(LogLevel::Error, "Unspecified parse error");
		break;
	}
}

void AppLayer::_OnLowerLayerUp()
{
	mpUser->OnLowerLayerUp();
}

void AppLayer::_OnLowerLayerDown()
{
	//reset both the channels
	mSolicited.Reset();
	mUnsolicited.Reset();

	//reset the transmitter state
	mSendQueue.erase(mSendQueue.begin(), mSendQueue.end());
	mSending = false;

	//notify the user
	mpUser->OnLowerLayerDown();
}

void AppLayer::OnSendResult(bool aSuccess)
{
	if(mSending)
	{
		assert(mSendQueue.size() > 0);
		mSending = false;

		FunctionCode func = mSendQueue.front().GetFunction();
		mSendQueue.pop_front();

		if (func == FunctionCode::CONFIRM)
		{
			assert(mConfirmSending);
			mConfirmSending = false;
		}
		else
		{
			if (aSuccess)
			{
				if (func == FunctionCode::UNSOLICITED_RESPONSE) mUnsolicited.OnSendSuccess();
				else mSolicited.OnSendSuccess();
			}
			else
			{
				if (func == FunctionCode::UNSOLICITED_RESPONSE) mUnsolicited.OnSendFailure();
				else mSolicited.OnSendFailure();
			}
		}

		this->CheckForSend();
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Layer is not sending");
	}
}

void AppLayer::_OnSendSuccess()
{
	this->OnSendResult(true);
}

void AppLayer::_OnSendFailure()
{
	this->OnSendResult(false);
}


////////////////////
// Internal Events
////////////////////

void AppLayer::OnResponse(const APDUResponseRecord& record)
{
	if(record.control.UNS)
	{
		ERROR_BLOCK(LogLevel::Warning, "Bad unsol bit", ALERR_BAD_UNSOL_BIT);
	}
	else
	{
		// If we get a response that requests confirmation, we shouldn't confirm
		// if we're not going to handle the data. This is usually indicative of an
		// early timeout. It will show up in the logs as a response without context.
		if(record.control.CON && mSolicited.AcceptsResponse())
		{
			this->QueueConfirm(false, record.control.SEQ);
		}

		mSolicited.OnResponse(record);
	}
}

void AppLayer::OnUnsolResponse(const APDUResponseRecord& record)
{
	if(!record.control.UNS)
	{
		ERROR_BLOCK(LogLevel::Warning, "Unsolicited response code with uns bit not set", ALERR_BAD_UNSOL_BIT);
	}
	else
	{

		if(record.control.CON)
			this->QueueConfirm(true, record.control.SEQ);

		mUnsolicited.OnUnsol(record);
	}


}

void AppLayer::OnConfirm(const AppControlField& aControl, size_t aDataSize)
{
	if(aDataSize > 0)
	{
		LOG_BLOCK(LogLevel::Warning, "Unexpected payload in confirm of size: " << aDataSize);
	}
	else
	{
		if(aControl.UNS) // which channel?
		{
			if(mIsMaster)
			{
				ERROR_BLOCK(LogLevel::Error, "Unexpcted confirm for master", ALERR_UNEXPECTED_CONFIRM)
			}
			else
			{
				mUnsolicited.OnConfirm(aControl.SEQ);
			}
		}
		else
		{
			mSolicited.OnConfirm(aControl.SEQ);
		}
	}
}

void AppLayer::OnRequest(const APDURecord& record)
{
	if(record.control.UNS)
	{
		ERROR_BLOCK(LogLevel::Warning, "Received request with UNS bit", ALERR_BAD_UNSOL_BIT);
	}
	else
	{
		if(record.control.IsFirAndFin()) mSolicited.OnRequest(record);
		else
		{
			ERROR_BLOCK(LogLevel::Warning,  "Received non FIR/FIN request", ALERR_MULTI_FRAGEMENT_REQUEST);
		}
	}
}

////////////////////
// Helpers
////////////////////

void AppLayer::QueueConfirm(bool aUnsol, int aSeq)
{
	if(mConfirmSending)
	{
		ERROR_BLOCK(LogLevel::Warning, "Confirm request flood, ignoring confirm", aUnsol ? ALERR_UNSOL_FLOOD : ALERR_SOL_FLOOD);
	}
	else
	{
		mConfirmSending = true;
		AppControlField acf(true, true, false, aUnsol, aSeq);
		confirmAPDU.SetControl(acf);
		this->QueueFrame(confirmAPDU);
	}
}


void AppLayer::QueueFrame(const APDUWrapper& apdu)
{
	mSendQueue.push_back(apdu);
	this->CheckForSend();
}

void AppLayer::CheckForSend()
{
	if(!mSending && mSendQueue.size() > 0)
	{
		mSending = true;
		//LOG_BLOCK(LogLevel::Interpret, "=> AL " << pAPDU->ToString()); TODO - replace outgoing logging
		mpLowerLayer->Send(mSendQueue.front().ToReadOnly());
	}
}

size_t AppLayer::GetRetries(FunctionCode aCode)
{
	switch(aCode)
	{
	case(FunctionCode::DIRECT_OPERATE):
	case(FunctionCode::DIRECT_OPERATE_NO_ACK):
	case(FunctionCode::RESPONSE):
	case(FunctionCode::WRITE): // b/c these can contain time objects which are sensitive to retries
		return 0;
	default:
		return mNumRetry; //use the configured
	}
}

} //end ns
