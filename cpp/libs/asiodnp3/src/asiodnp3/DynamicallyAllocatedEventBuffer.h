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

#ifndef __DYNAMICALLY_ALLOCATED_EVENT_BUFFER_H_
#define __DYNAMICALLY_ALLOCATED_EVENT_BUFFER_H_

#include <opendnp3/outstation/EventBufferConfig.h>
#include <opendnp3/outstation/EventBufferFacade.h>

#include <openpal/container/DynamicArray.h>

namespace asiodnp3
{

class DynamicallyAllocatedEventBuffer : openpal::Uncopyable
{

public:
	DynamicallyAllocatedEventBuffer(const opendnp3::EventBufferConfig& config);

	opendnp3::EventBufferFacade GetFacade();

private:
	openpal::DynamicArray<uint16_t, uint16_t> binaryStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::Binary>, uint16_t> binaryArray;

	openpal::DynamicArray<uint16_t, uint16_t> doubleBinaryStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::DoubleBitBinary>, uint16_t> doubleBinaryArray;

	openpal::DynamicArray<uint16_t, uint16_t> analogStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::Analog>, uint16_t> analogArray;

	openpal::DynamicArray<uint16_t, uint16_t> counterStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::Counter>, uint16_t> counterArray;

	openpal::DynamicArray<uint16_t, uint16_t> frozenCounterStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::FrozenCounter>, uint16_t> frozenCounterArray;

	openpal::DynamicArray<uint16_t, uint16_t> binaryOutputStatusStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::BinaryOutputStatus>, uint16_t> binaryOutputStatusArray;

	openpal::DynamicArray<uint16_t, uint16_t> analogOutputStatusStack;
	openpal::DynamicArray<opendnp3::Event<opendnp3::AnalogOutputStatus>, uint16_t> analogOutputStatusArray;

	openpal::DynamicArray<openpal::ListNode<opendnp3::SequenceRecord>, uint16_t> sequenceOfEvents;
	openpal::DynamicArray<openpal::ListNode<opendnp3::SequenceRecord>*, uint16_t> selectedEvents;
};

}

#endif
