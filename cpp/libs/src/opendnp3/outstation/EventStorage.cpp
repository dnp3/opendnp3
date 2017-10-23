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

#include "EventStorage.h"

namespace opendnp3
{

EventStorage::EventStorage(const EventBufferConfig& config) :
	events(config.TotalEvents()),
	binary(config.maxBinaryEvents),
	doubleBinary(config.maxDoubleBinaryEvents),
	analog(config.maxAnalogEvents),
	counter(config.maxCounterEvents),
	frozenCounter(config.maxFrozenCounterEvents),
	binaryOutputStatus(config.maxBinaryOutputStatusEvents),
	analogOutputStatus(config.maxAnalogOutputStatusEvents)
{}

bool EventStorage::Update(const Event<BinarySpec>& evt)
{
	return this->UpdateAny(evt, this->binary);
}

bool EventStorage::Update(const Event<DoubleBitBinarySpec>& evt)
{
	return this->UpdateAny(evt, this->doubleBinary);
}

bool EventStorage::Update(const Event<AnalogSpec>& evt)
{
	return this->UpdateAny(evt, this->analog);
}

bool EventStorage::Update(const Event<CounterSpec>& evt)
{
	return this->UpdateAny(evt, this->counter);
}

bool EventStorage::Update(const Event<FrozenCounterSpec>& evt)
{
	return this->UpdateAny(evt, this->frozenCounter);
}

bool EventStorage::Update(const Event<BinaryOutputStatusSpec>& evt)
{
	return this->UpdateAny(evt, this->binaryOutputStatus);
}

bool EventStorage::Update(const Event<AnalogOutputStatusSpec>& evt)
{
	return this->UpdateAny(evt, this->analogOutputStatus);
}

}


