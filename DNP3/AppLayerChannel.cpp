
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "AppLayerChannel.h"

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>
#include <APL/IExecutor.h>

#include "AppLayer.h"
#include "AppChannelStates.h"

#include <functional>

namespace apl
{
namespace dnp
{

AppLayerChannel::AppLayerChannel(const std::string& arName, Logger* apLogger, AppLayer* apAppLayer, IExecutor* apExecutor, millis_t aTimeout) :
	Loggable(apLogger),
	mpAppLayer(apAppLayer),
	mpSendAPDU(NULL),
	mNumRetry(0),
	mpExecutor(apExecutor),
	mpTimer(NULL),
	M_TIMEOUT(aTimeout),
	M_NAME(arName)
{
	this->Reset();
}

void AppLayerChannel::Reset()
{
	mpState = ACS_Idle::Inst();
	mConfirming = false;
	mSequence = -1;
	if(mpTimer) {
		mpTimer->Cancel();
		mpTimer = NULL;
	}
}

// ---- Events ----

void AppLayerChannel::Send(APDU& arAPDU, size_t aNumRetry)
{
	mpState->Send(this, arAPDU, aNumRetry);
	mpSendAPDU = &arAPDU;
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

void AppLayerChannel::QueueSend(APDU& arAPDU)
{
	mpAppLayer->QueueFrame(arAPDU);
}

bool AppLayerChannel::Retry(ACS_Base* apState)
{
	if(mNumRetry > 0) {
		--mNumRetry;
		LOG_BLOCK(LEV_INFO, "App layer retry, " << mNumRetry << " remaining");
		this->ChangeState(apState);
		mpAppLayer->QueueFrame(*mpSendAPDU);
		return true;
	}
	else {
		return false;
	}
}

void AppLayerChannel::DoPartialResponse(APDU& arAPDU)
{
	mpAppLayer->mpUser->OnPartialResponse(arAPDU);
}

void AppLayerChannel::DoFinalResponse(APDU& arAPDU)
{
	mpAppLayer->mpUser->OnFinalResponse(arAPDU);
}

void AppLayerChannel::StartTimer()
{
	if(mpTimer != NULL) throw InvalidStateException(LOCATION, "");
	mpTimer = mpExecutor->Start(std::chrono::milliseconds(M_TIMEOUT), std::bind(&AppLayerChannel::Timeout, this));
}

void AppLayerChannel::CancelTimer()
{
	if(mpTimer == NULL) throw InvalidStateException(LOCATION, "");
	mpTimer->Cancel();
	mpTimer = NULL;
}

void AppLayerChannel::ChangeState(ACS_Base* apState)
{
	if(apState != mpState) {
		LOG_BLOCK(LEV_DEBUG, "State changed from " << mpState->Name() << " to " << apState->Name());
		mpState = apState;
	}
}

void AppLayerChannel::Timeout()
{
	mpTimer = NULL;
	mpState->OnTimeout(this);
}

}
}


