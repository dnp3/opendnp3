/*
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
#ifndef OPENDNP3_EVENTLISTS_H
#define OPENDNP3_EVENTLISTS_H

#include "opendnp3/outstation/EventBufferConfig.h"

#include "openpal/util/Uncopyable.h"

#include "TypedEventRecord.h"
#include "EventRecord.h"
#include "ClazzCount.h"

namespace opendnp3
{

struct EventLists : private openpal::Uncopyable
{

	EventLists() = delete;

	EventLists(const EventBufferConfig& config);

	// master list keeps the aggregate order and generic data
	openpal::LinkedList<EventRecord, uint32_t> events;

	template <class T>
	openpal::LinkedList<TypedEventRecord<T>, uint32_t>& GetList();

	bool RemoveTypeStorage(EventRecord& node);

	bool IsAnyTypeFull() const;

	EventClassCounters counters;	

private:	
	
	template <class T>
	void RemoveType(EventRecord& record);

	// sub-lists just act as type-specific storage
	openpal::LinkedList<TypedEventRecord<BinarySpec>, uint32_t> binary;
	openpal::LinkedList<TypedEventRecord<DoubleBitBinarySpec>, uint32_t> doubleBinary;
	openpal::LinkedList<TypedEventRecord<AnalogSpec>, uint32_t> analog;
	openpal::LinkedList<TypedEventRecord<CounterSpec>, uint32_t> counter;
	openpal::LinkedList<TypedEventRecord<FrozenCounterSpec>, uint32_t> frozenCounter;
	openpal::LinkedList<TypedEventRecord<BinaryOutputStatusSpec>, uint32_t> binaryOutputStatus;
	openpal::LinkedList<TypedEventRecord<AnalogOutputStatusSpec>, uint32_t> analogOutputStatus;	
};

}

#endif

