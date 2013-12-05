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
#ifndef __EVENT_TYPES_H_
#define __EVENT_TYPES_H_

#include "DNPDatabaseTypes.h"

#include <opendnp3/VTOData.h>

#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * Structure for holding event data information. Adds a sequence number and a
 * written flag to the data member.
 */
template<typename T>
struct DLL_LOCAL EventInfo : public PointInfoBase<T> {
	EventInfo(const T& arValue, PointClass aClass, size_t aIndex) :
		PointInfoBase<T>(arValue, aClass, aIndex),
		mSequence(0),
		mWritten(false)
	{}

	EventInfo() : mSequence(0), mWritten(false) {}

	size_t mSequence;	// sequence number used by the event buffers to record insertion order
	bool mWritten;		// true if the event has been written
};

typedef EventInfo<Binary>				BinaryEvent;
typedef EventInfo<Analog>				AnalogEvent;
typedef EventInfo<Counter>				CounterEvent;
typedef EventInfo<VtoData>				VtoEvent;

template <typename EventType>
struct DLL_LOCAL EvtItr {
	typedef typename std::vector< EventType >::iterator Type;
};

typedef EvtItr<BinaryEvent>::Type			BinaryEventIter;
typedef EvtItr<AnalogEvent>::Type			AnalogEventIter;
typedef EvtItr<CounterEvent>::Type			CounterEventIter;
typedef EvtItr<VtoEvent>::Type				VtoDataEventIter;

} //end namespace

/* vim: set ts=4 sw=4: */

#endif
