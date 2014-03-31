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

#include "EventBufferFacade.h"

namespace opendnp3
{

SequenceRecord::SequenceRecord() : type(EventType::Analog), index(0), clazz(EventClass::EC1), selected(false)
{}

SequenceRecord::SequenceRecord(EventType type_, uint16_t index_, EventClass clazz_, bool selected_) :
	type(type_), index(index_), clazz(clazz_), selected(selected_)
{}

EventBufferFacade::EventBufferFacade(
	openpal::RandomInsertAdapter<Event<Binary>, uint16_t> binaryEvents_,
	openpal::RandomInsertAdapter<Event<DoubleBitBinary>, uint16_t> doubleBinaryEvents_,
	openpal::RandomInsertAdapter<Event<Analog>, uint16_t> analogEvents_,
	openpal::RandomInsertAdapter<Event<Counter>, uint16_t> counterEvents_,
	openpal::RandomInsertAdapter<Event<FrozenCounter>, uint16_t> frozenCounterEvents_,
	openpal::RandomInsertAdapter<Event<BinaryOutputStatus>, uint16_t> binaryOutputStatusEvents_,
	openpal::RandomInsertAdapter<Event<AnalogOutputStatus>, uint16_t> analogOutputStatusEvents_,
						
	openpal::LinkedListAdapter<SequenceRecord, uint16_t> sequenceOfEvents_,
	openpal::StackAdapter<openpal::ListNode<SequenceRecord>*, uint16_t> selectedEvents_) :

	binaryEvents(binaryEvents_),
	doubleBinaryEvents(doubleBinaryEvents_),
	analogEvents(analogEvents_),
	counterEvents(counterEvents_),
	frozenCounterEvents(frozenCounterEvents_),
	binaryOutputStatusEvents(binaryOutputStatusEvents_),
	analogOutputStatusEvents(analogOutputStatusEvents_),
	sequenceOfEvents(sequenceOfEvents_),
	selectedEvents(selectedEvents_)
{

}

}

