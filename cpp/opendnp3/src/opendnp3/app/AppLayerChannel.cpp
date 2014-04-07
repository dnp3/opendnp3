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
#include "AppLayerChannel.h"

#include <openpal/LogMacros.h>
#include <openpal/IExecutor.h>
#include <openpal/Bind.h>

#include "AppLayer.h"
#include "AppChannelStates.h"
#include "opendnp3/LogLevels.h"

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

AppLayerChannel::AppLayerChannel(const openpal::Logger& logger_, AppLayer* apAppLayer, IExecutor* apExecutor, TimeDuration aTimeout) :	
	logger(logger_),
	mpAppLayer(apAppLayer),
	mNumRetry(0),
	mpExecutor(apExecutor),
	mpTimer(nullptr),
	M_TIMEOUT(aTimeout)	
{
	this->Reset();
}

void AppLayerChannel::Reset()
{
	mpState = ACS_Idle::Inst();
	mConfirming = false;
	mSequence = -1;
	if(mpTimer)
	{
		mpTimer->Cancel();
		mpTimer = nullptr;
	}
}

// ---- Events ----

void AppLayerChannel::Send(APDUWrapper& apdu, uint32_t aNumRetry)
{
	mpState->Send(this, apdu, aNumRetry);
}

void AppLayerChannel::OnSendSuccess()
{
	mpState->OnSendSuccess(this);
}

void AppLayerChannel::OnSendFailure()
{
	mpState->OnSendFailure(this);
}

void AppLayerChannel::OnConfirm(int aSeq)
{
	mpState->OnConfirm(this, aSeq);
}

void AppLayerChannel::Cancel()
{
	mpState->Cancel(this);
}


// ---- State Actions ----

void AppLayerChannel::QueueSend(APDUWrapper& apdu)
{
	mpAppLayer->QueueFrame(apdu);
}

bool AppLayerChannel::Retry(ACS_Base* apState)
{
	if(mNumRetry > 0)
	{
		--mNumRetry;
		FORMAT_LOG_BLOCK(logger, flags::INFO, "App layer retry, %u remaining", static_cast<unsigned int>(mNumRetry));
		this->ChangeState(apState);
		mpAppLayer->QueueFrame(mSendAPDU);
		return true;
	}
	else
	{
		return false;
	}
}

void AppLayerChannel::DoPartialResponse(const APDUResponseRecord& record)
{
	mpAppLayer->pUser->OnPartialResponse(record);
}

void AppLayerChannel::DoFinalResponse(const APDUResponseRecord& record)
{
	mpAppLayer->pUser->OnFinalResponse(record);
}

void AppLayerChannel::StartTimer()
{
	assert(mpTimer == nullptr);
	auto lambda = [this]() { this->Timeout(); };
	mpTimer = mpExecutor->Start(TimeDuration(M_TIMEOUT), openpal::Bind(lambda));
}

void AppLayerChannel::CancelTimer()
{
	assert(mpTimer != nullptr);
	mpTimer->Cancel();
	mpTimer = nullptr;
}

void AppLayerChannel::ChangeState(ACS_Base* apState)
{
	if(apState != mpState)
	{
		FORMAT_LOG_BLOCK(logger, flags::DBG, "State changed from %s to %s", mpState->Name(), apState->Name());
		mpState = apState;
	}
}

void AppLayerChannel::Timeout()
{
	mpTimer = nullptr;
	mpState->OnTimeout(this);
}

}


