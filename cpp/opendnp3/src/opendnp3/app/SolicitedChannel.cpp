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
#include "SolicitedChannel.h"

#include "AppLayer.h"
#include "AppChannelStates.h"

#include <openpal/LoggableMacros.h>


namespace opendnp3
{


SolicitedChannel::SolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, IExecutor* apExecutor, openpal::TimeDuration aTimeout) :
	AppLayerChannel("Solicited", aLogger, apApp, apExecutor, aTimeout)
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

void SolicitedChannel::OnResponse(const APDUResponseRecord& aRecord)
{
	mpState->OnResponse(this, aRecord);
}

void SolicitedChannel::OnRequest(const APDURecord& aRecord)
{
	auto seq = SequenceInfo::OTHER;
	if (aRecord.control.SEQ == this->Sequence())
	{
		LOG_BLOCK(LogLevel::Warning, "Received previous sequence");
		seq = SequenceInfo::PREVIOUS;
	}
	else if (aRecord.control.SEQ == NextSeq(this->Sequence()))
	{
		seq = SequenceInfo::CORRECT;
	}

	mSequence = aRecord.control.SEQ;

	mpAppLayer->mpUser->OnRequest(aRecord, seq);
}

}


