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

#include "EventLists.h"

namespace opendnp3
{

EventLists::EventLists(const EventBufferConfig& config) :
	events(config.TotalEvents()),
	binary(config.maxBinaryEvents),
	doubleBinary(config.maxDoubleBinaryEvents),
	analog(config.maxAnalogEvents),
	counter(config.maxCounterEvents),
	frozenCounter(config.maxFrozenCounterEvents),
	binaryOutputStatus(config.maxBinaryOutputStatusEvents),
	analogOutputStatus(config.maxAnalogOutputStatusEvents)
{}

bool EventLists::IsAnyTypeFull() const
{
	return
	    this->binary.IsFull() ||
	    this->doubleBinary.IsFull() ||
	    this->counter.IsFull() ||
	    this->frozenCounter.IsFull() ||
	    this->analog.IsFull() ||
	    this->binaryOutputStatus.IsFull() ||
	    this->analogOutputStatus.IsFull();
}

template <>
openpal::LinkedList<TypedEventRecord<BinarySpec>, uint32_t>& EventLists::GetList()
{
	return this->binary;
}

template <>
openpal::LinkedList<TypedEventRecord<DoubleBitBinarySpec>, uint32_t>& EventLists::GetList()
{
	return this->doubleBinary;
}

template <>
openpal::LinkedList<TypedEventRecord<CounterSpec>, uint32_t>& EventLists::GetList()
{
	return this->counter;
}

template <>
openpal::LinkedList<TypedEventRecord<FrozenCounterSpec>, uint32_t>& EventLists::GetList()
{
	return this->frozenCounter;
}

template <>
openpal::LinkedList<TypedEventRecord<AnalogSpec>, uint32_t>& EventLists::GetList()
{
	return this->analog;
}

template <>
openpal::LinkedList<TypedEventRecord<BinaryOutputStatusSpec>, uint32_t>& EventLists::GetList()
{
	return this->binaryOutputStatus;
}

template <>
openpal::LinkedList<TypedEventRecord<AnalogOutputStatusSpec>, uint32_t>& EventLists::GetList()
{
	return this->analogOutputStatus;
}

}



