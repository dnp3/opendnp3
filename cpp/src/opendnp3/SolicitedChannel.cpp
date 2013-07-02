
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
// you under the terms of the License.
//

#include "SolicitedChannel.h"

#include "AppLayer.h"
#include "AppChannelStates.h"
#include "LoggableMacros.h"

#include <opendnp3/Logger.h>

namespace opendnp3
{


SolicitedChannel::SolicitedChannel(Logger* apLogger, AppLayer* apApp, IExecutor* apExecutor, millis_t aTimeout) :
	AppLayerChannel("Solicited", apLogger, apApp, apExecutor, aTimeout)
{}

bool SolicitedChannel::AcceptsResponse()
{
	return mpState->AcceptsResponse();
}

void SolicitedChannel::DoSendSuccess()
{
	mpAppLayer->mpUser->OnSolSendSuccess();
}

void SolicitedChannel::DoFailure()
{
	mpAppLayer->mpUser->OnSolFailure();
}

void SolicitedChannel::OnResponse(APDU& arAPDU)
{
	mpState->OnResponse(this, arAPDU);
}

void SolicitedChannel::OnRequest(APDU& arAPDU)
{

	AppControlField acf = arAPDU.GetControl();

	SequenceInfo seq = SI_OTHER;
	if (acf.SEQ == this->Sequence()) {
		LOG_BLOCK(LEV_WARNING, "Received previous sequence");
		seq = SI_PREV;
	}
	else if (acf.SEQ == NextSeq(this->Sequence())) {
		seq = SI_CORRECT;
	}

	mSequence = acf.SEQ;

	mpAppLayer->mpUser->OnRequest(arAPDU, seq);
}

void SolicitedChannel::OnUnknownObjectInRequest(const AppControlField& acf)
{
	mSequence = acf.SEQ; // capture the sequence number
}

}


