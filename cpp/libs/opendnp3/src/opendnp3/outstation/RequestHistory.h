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
#ifndef OPENDNP3_REQUESTHISTORY_H
#define OPENDNP3_REQUESTHISTORY_H

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUWrapper.h"

#include "opendnp3/outstation/DeferredRequest.h"

#include <cstdint>

#include <openpal/container/DynamicBuffer.h>

namespace opendnp3
{

/// Tracks the state of the last request ASDU
class RequestHistory
{	
	public:

	/// describes the various states
	enum class State
	{
		FIRST,		// no previous requests
		PREVIOUS,	// A previously processed request is buffered for comparison purposes
		DEFERED		// A defered request is buffered to be processed when possible
	};

	RequestHistory(uint32_t maxFragSize);

	void Reset();
	State CurrentState() const;
	APDUEquality RecordLastRequest(const APDUHeader& header, const openpal::ReadBufferView& objects);
	void DeferRequest(const APDUHeader& header, const openpal::ReadBufferView& objects, bool isRepeat, bool objectsEqualToLast);

	DeferredRequest GetDeferedRequest();
	void ClearDeferedRequest();

	private:

	State state;		
	DeferredRequest defered;	   // info about the stored request
	openpal::DynamicBuffer buffer; // the underlying storage
};


}



#endif

