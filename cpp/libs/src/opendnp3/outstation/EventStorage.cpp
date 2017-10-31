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

#include "EventSelection.h"
#include "EventUpdate.h"

namespace opendnp3
{

EventStorage::EventStorage(const EventBufferConfig& config) :
	state(config)
{}

bool EventStorage::Update(const Event<BinarySpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<DoubleBitBinarySpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<AnalogSpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<CounterSpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<FrozenCounterSpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<BinaryOutputStatusSpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

bool EventStorage::Update(const Event<AnalogOutputStatusSpec>& evt)
{
	return EventUpdate::Update(state, evt);
}

uint32_t EventStorage::SelectByType(EventBinaryVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<BinarySpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventDoubleBinaryVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<DoubleBitBinarySpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventAnalogVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<AnalogSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventCounterVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<CounterSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventFrozenCounterVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<FrozenCounterSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByType(EventBinaryOutputStatusVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<BinaryOutputStatusSpec>(this->state, variation, max); return 0;
}

uint32_t EventStorage::SelectByType(EventAnalogOutputStatusVariation variation, uint32_t max)
{
	return EventSelection::SelectByType<AnalogOutputStatusSpec>(this->state, variation, max);
}

uint32_t EventStorage::SelectByClass(EventClass clazz, uint32_t max)
{
	uint32_t num_selected = 0;
	auto iter = this->state.events.Iterate();

	while (iter.HasNext() && num_selected < max)
	{
		auto node = iter.Next();
		auto record = node->value;
		if (node->value.state == EventState::unselected && node->value.clazz == clazz)
		{
			// if not previously selected
			node->value.state = EventState::selected;
			// TODO - set the storage to use the default variation
			//node->value.selectedVariation = useDefaultVariation ? node->value.defaultVariation : variation;
			++num_selected;
		}
	}

	return num_selected;
}


uint32_t EventStorage::Write(IEventWriteHandler& handler)
{
	return 0;
}

}


