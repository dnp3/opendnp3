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
#include "AppChannelStates.h"

#include <openpal/LoggableMacros.h>
#include <openpal/IExecutor.h>

#include "opendnp3/Singleton.h"
#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/app/AppLayerChannel.h"


#include <string>
#include <sstream>

using namespace openpal;

namespace opendnp3
{

// ---- Default behaviors for the states ----

void ACS_Base::Send(AppLayerChannel* c, APDUWrapper&, size_t)
{
	LOGGER_BLOCK(c->logger, LogLevel::Error, "Invalid action for state: " << this->Name());
}

void ACS_Base::Cancel(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->logger, LogLevel::Error, "Invalid action for state: " << this->Name());
}

void ACS_Base::OnSendSuccess(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->logger, LogLevel::Error, "Invalid action for state: " << this->Name());
}

void ACS_Base::OnSendFailure(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->logger, LogLevel::Error, "Invalid action for state: " << this->Name());
}

void ACS_Base::OnConfirm(AppLayerChannel* c, int aSeq)
{
	ERROR_LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning,
	                   "Unexpected confirm with sequence: " << aSeq, ALERR_UNEXPECTED_CONFIRM);
}

void ACS_Base::OnResponse(AppLayerChannel* c, const APDUResponseRecord& rsp)
{
	LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning,
	             "Unexpected response with sequence: " << static_cast<int>(rsp.control.SEQ));
}

void ACS_Base::OnTimeout(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->logger, LogLevel::Error, "Invalid action for state: " << this->Name());
}

void ACS_Base::ProcessResponse(AppLayerChannel* c, const APDUResponseRecord& record, bool aExpectFIR)
{
	if(record.control.SEQ == c->Sequence())
	{
		if(record.control.FIR == aExpectFIR)
		{
			c->CancelTimer();

			if(record.control.FIN)
			{
				c->ChangeState(ACS_Idle::Inst());
				c->DoFinalResponse(record);
			}
			else
			{
				c->IncrSequence();
				c->ChangeState(ACS_WaitForFinalResponse::Inst());
				c->StartTimer();
				c->DoPartialResponse(record);
			}
		}
		else
		{
			ERROR_LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning, "Unexpected fir bit " << record.control.FIR, ALERR_BAD_FIR_FIN);
		}
	}
	else
	{
		ERROR_LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning, "Bad sequence number " << static_cast<int>(record.control.SEQ), ALERR_BAD_SEQUENCE);
	}
}

// ---- ACS_IDle ----

ACS_Idle ACS_Idle::mInstance;

void ACS_Idle::Send(AppLayerChannel* c, APDUWrapper& apdu, size_t aNumRetry)
{
	AppControlField acf = apdu.GetControl();
	FunctionCode func = apdu.GetFunction();
	acf.SEQ = (acf.FIR && func == FunctionCode::RESPONSE) ? c->Sequence() : c->IncrSequence();
	apdu.SetControl(acf);
	auto pNext = NextState(c, func, acf.CON);
	if (pNext == this)
	{
		c->mpExecutor->Post([c]()
		{
			c->DoFailure();
		});
	}
	else
	{
		c->mSendAPDU = apdu; // assign this record so we can retry
		c->ChangeState(pNext);
		c->SetRetry(aNumRetry);
		c->QueueSend(apdu);
	}
}

ACS_Base* ACS_Idle::NextState(AppLayerChannel* c, FunctionCode aFunc, bool aConfirm)
{
	switch(aFunc)
	{
	case(FunctionCode::CONFIRM) :
		LOGGER_BLOCK(c->logger, LogLevel::Error, "Cannot send a confirm manually");
		return this;
	case(FunctionCode::RESPONSE):
		if(c->Sequence() < 0)
		{
			LOGGER_BLOCK(c->logger, LogLevel::Error, "Can't respond until we've received a request");
			return this;
		}
		else
		{
			if (aConfirm) return ACS_SendConfirmed::Inst();
			else return ACS_Send::Inst();
		}
	case(FunctionCode::UNSOLICITED_RESPONSE):
		if(aConfirm) return ACS_SendConfirmed::Inst();
		else return ACS_Send::Inst();

	case(FunctionCode::DIRECT_OPERATE_NO_ACK):
		if(aConfirm)
		{
			LOGGER_BLOCK(c->logger, LogLevel::Error, "DO no ACK can't be confirmed");
		}
		return ACS_Send::Inst();

	default:	// it's a request with an expected response
		if(aConfirm)
		{
			LOGGER_BLOCK(c->logger, LogLevel::Error, "Confirmation not allowed for requests");
		}
		return ACS_SendExpectResponse::Inst();
	}
}

// ---- ACS_SendBase ----

void ACS_SendBase::OnSendFailure(AppLayerChannel* c)
{
	if(!c->Retry(this))   //if we can't retry, then go back to idle
	{
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

void ACS_SendBase::Cancel(AppLayerChannel* c)
{
	c->ChangeState(ACS_SendCanceled::Inst());
}

// ---- ACS_SendCanceled ----

ACS_SendCanceled ACS_SendCanceled::mInstance;

void ACS_SendCanceled::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoFailure();
}

void ACS_SendCanceled::OnSendFailure(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoFailure();
}

// ---- ACS_Send ----

ACS_Send ACS_Send::mInstance;

void ACS_Send::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->DoSendSuccess();
}

// ---- ACS_SendConfirmed ----

ACS_SendConfirmed ACS_SendConfirmed::mInstance;

void ACS_SendConfirmed::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_WaitForConfirm::Inst());
	c->StartTimer();
}

// ----  ACS_SendExpectResponse ----

ACS_SendExpectResponse  ACS_SendExpectResponse::mInstance;

void ACS_SendExpectResponse::OnSendSuccess(AppLayerChannel* c)
{
	c->ChangeState(ACS_WaitForFirstResponse::Inst());
	c->StartTimer();
}

// ---- ACS_WaitForConfirm ----

ACS_WaitForConfirm ACS_WaitForConfirm::mInstance;

void ACS_WaitForConfirm::Cancel(AppLayerChannel* c)
{
	c->ChangeState(ACS_Idle::Inst());
	c->CancelTimer();
	c->DoFailure();
}

void ACS_WaitForConfirm::OnConfirm(AppLayerChannel* c, int aSeq)
{
	// does the confirm sequence match what we expect?
	if(c->Sequence() == aSeq)
	{
		c->CancelTimer();
		c->ChangeState(ACS_Idle::Inst());
		c->DoSendSuccess();
	}
	else
	{
		ERROR_LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning,
		                   "Unexpected confirm w/ sequence " << aSeq, ALERR_UNEXPECTED_CONFIRM);
	}

}

void ACS_WaitForConfirm::OnTimeout(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning, "Timeout while waiting for confirm");
	if(!c->Retry(ACS_SendConfirmed::Inst()))
	{
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

// ---- ACS_WaitForResponseBase ----

void ACS_WaitForResponseBase::OnTimeout(AppLayerChannel* c)
{
	LOGGER_BLOCK(c->GetLogger(), LogLevel::Warning, "Timeout while waiting for response");
	if(!c->Retry(ACS_SendExpectResponse::Inst()))
	{
		c->ChangeState(ACS_Idle::Inst());
		c->DoFailure();
	}
}

// ---- ACS_WaitForFirstResponse ----

ACS_WaitForFirstResponse ACS_WaitForFirstResponse::mInstance;

void ACS_WaitForFirstResponse::OnResponse(AppLayerChannel* c, const APDUResponseRecord& record)
{
	this->ProcessResponse(c, record, true);
}

// ---- ATS_WaitForFinalResponse ----

ACS_WaitForFinalResponse ACS_WaitForFinalResponse::mInstance;

void ACS_WaitForFinalResponse::OnResponse(AppLayerChannel* c, const APDUResponseRecord& record)
{
	this->ProcessResponse(c, record, false);
}

}

