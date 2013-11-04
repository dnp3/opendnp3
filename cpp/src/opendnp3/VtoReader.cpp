
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

#include "VtoReader.h"

#include <opendnp3/DNPConstants.h>
#include <openpal/Exception.h>

#include <openpal/LoggableMacros.h>

#include "EnhancedVto.h"


using namespace openpal;

namespace opendnp3
{

void VtoReader::AddVtoChannel(IVtoDataHandler* apCallbacks)
{
	/*
	 * The whole function is thread-safe, from start to finish.
	 */
	std::unique_lock<std::mutex> lock(mMutex);

	uint8_t id = apCallbacks->GetChannelId();

	/* Has this channel id already been registered? */
	if (mChannelMap.find(id) != mChannelMap.end()) {
		std::stringstream out;
		out << (int)id;

		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Channel already registered: " << out.str() );
	}

	/* Register the callbacks for the channel id */
	mChannelMap[id] = apCallbacks;
}

void VtoReader::RemoveVtoChannel(IVtoDataHandler* apCallbacks)
{
	/*
	 * The whole function is thread-safe, from start to finish.
	 */
	std::unique_lock<std::mutex> lock(mMutex);

	uint8_t id = apCallbacks->GetChannelId();

	if (mChannelMap.erase(id) == 0) {		
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Channel not registered: " << ((int) id));
	}
}

void VtoReader::UpdateEnhancedVto(const VtoData& arData, uint8_t aChannelId)
{
	bool remoteOnline;
	uint8_t realChannel;
	try {
		EnhancedVto::ReadVtoData(arData, remoteOnline, realChannel);
		ChannelMap::iterator i = mChannelMap.find(realChannel);

		if (i == mChannelMap.end()) {
			ERROR_BLOCK(LEV_ERROR,
			            "Received enhanced Vto control message for unregistered channel: " << realChannel,
			            VTOERR_ENHANCED_VTO_FOR_UNREGISTERED_CHANNEL);
		}
		else {
			VtoData data(remoteOnline ? VTODT_REMOTE_OPENED : VTODT_REMOTE_CLOSED);
			i->second->OnVtoDataReceived(data);
		}
	}
	catch(const Exception& ex) {
		ERROR_BLOCK(LEV_WARNING, ex.GetErrorString(), VTOERR_BADLY_FORMATTED_ENHANCED_VTO);
	}
}

void VtoReader::UpdateNormalVto(const VtoData& arData, uint8_t aChannelId)
{
	ChannelMap::iterator i = mChannelMap.find(aChannelId);

	if (i == mChannelMap.end()) {
		ERROR_BLOCK(LEV_ERROR,
		            "No registered callback handler for received data "
		            "on VTO channel id: " + aChannelId,
		            VTOERR_VTO_FOR_UNEXPECTED_CHANNEL);
	}
	else {
		i->second->OnVtoDataReceived(arData);
	}
}

void VtoReader::Update(const VtoData& arData,
                       uint8_t aChannelId)
{
	/* Make sure we are part of the larger DNP3 transaction */
	assert( this->InProgress() );

	if(aChannelId == 255) {
		this->UpdateEnhancedVto(arData, aChannelId);
	}
	else {
		this->UpdateNormalVto(arData, aChannelId);
	}
}

void VtoReader::_Start()
{
	mMutex.lock();
}

void VtoReader::_End()
{
	mMutex.unlock();
}

}

/* vim: set ts=4 sw=4: */


