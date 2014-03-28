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
#ifndef __EVENT_RESPONSE_CONTEXT_H_
#define __EVENT_RESPONSE_CONTEXT_H_

#include <openpal/Uncopyable.h>

#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/OutstationEventBuffer.h"
#include "opendnp3/outstation/SelectionCriteria.h"

namespace opendnp3
{

/**
 * Builds and tracks the state of multi-fragmented event responses to READ requests,
 * coordinating with the event buffer
 */
class EventResponseContext : private openpal::Uncopyable
{
	
public:

	EventResponseContext(OutstationEventBuffer& buffer_);

	bool IsComplete() const;

	IINField ReadAll(const GroupVariationRecord& record);

	void Reset();

	// return true, if all events were loaded, false otherwise
	bool Load(ObjectWriter& writer);

private:

	SelectionCriteria criteria;	
	OutstationEventBuffer& buffer;

	// true if the event buffer was exhausted, false if apdu is full
	bool Iterate(ObjectWriter& writer, SelectionIterator& iterator);
	
	// return true true, if there is still room in APDU, false otherwise
	template <class T>
	bool WriteFullHeader(ObjectWriter& writer, SelectionIterator& iterator, IDNP3Serializer<T>* pSerializer);
};

template <class T>
bool EventResponseContext::WriteFullHeader(ObjectWriter& ow, SelectionIterator& iterator, IDNP3Serializer<T>* pSerializer)
{
	auto writer = ow.IterateOverCountWithPrefix<openpal::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX, pSerializer);
	if (writer.IsNull())
	{
		return false;
	}
	else
	{
		Event<T> evt;
		while (iterator.Read(evt))
		{
			if (writer.Write(evt.value, evt.index))
			{
				iterator.SelectCurrent();
				iterator.SeekNext();
			}
			else
			{
				writer.Complete();
				return false;
			}
		}
		writer.Complete();
		return true;
	}
}

}

#endif
