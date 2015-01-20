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

#ifndef OPENDNP3_DEFERREDREQUEST_H
#define OPENDNP3_DEFERREDREQUEST_H

#include "opendnp3/app/APDUHeader.h"

#include <openpal/container/DynamicBuffer.h>
#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

/**
* Records metadata about deferred requests
*/
class DeferredRequest : private openpal::Uncopyable
{

	public:

	DeferredRequest(uint32_t maxAPDUSize);

	void Reset();

	bool IsSet() const;

	FunctionCode GetFunction() const;

	void Set(APDUHeader header, openpal::ReadBufferView objects, bool equalsLastRequest);

	template <class Fun>
	void Process(const Fun& handler);
	
	private:

	DeferredRequest() = delete;

	bool isSet;
	bool equalsLastRequest;
	APDUHeader header;
	openpal::ReadBufferView objects;
	openpal::DynamicBuffer buffer;
	
};

template <class Fun>
void DeferredRequest::Process(const Fun& handler)
{
	if (isSet)
	{
		isSet = false;
		handler(header, objects, equalsLastRequest);
	}
}

}

#endif
