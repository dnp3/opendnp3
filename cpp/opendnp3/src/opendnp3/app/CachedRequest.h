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

#ifndef __CACHED_REQUEST_H_
#define __CACHED_REQUEST_H_

#include "opendnp3/Uncopyable.h"
#include "opendnp3/DynamicBuffer.h"

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/SequenceInfo.h"

#include <assert.h>

namespace opendnp3
{

/**
* Used to efficiently cache requests that the outstation can't process immediately
*/
class CachedRequest : private Uncopyable
{	
	public:

	CachedRequest(uint32_t aMaxFragmentSize);

	void Set(const APDURecord& aRecord, SequenceInfo aSequence);
	void Clear() { isCached = false; }

	bool IsSet() const { return isCached; }

	template <class ApplyFun>
	void Apply(const ApplyFun& fun);

	private:

	bool isCached;
	APDURecord record;
	SequenceInfo sequence;
	DynamicBuffer buffer;
};

template <class ApplyFun>
void CachedRequest::Apply(const ApplyFun& fun)
{
	assert(isCached);
	isCached = false;
	fun(record, sequence);
}

}

#endif
