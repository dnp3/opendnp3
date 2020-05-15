/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_EVENTLISTS_H
#define OPENDNP3_EVENTLISTS_H

#include "ClazzCount.h"
#include "EventRecord.h"
#include "TypedEventRecord.h"
#include "app/MeasurementTypeSpecs.h"

#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/util/Uncopyable.h"

namespace opendnp3
{

typedef List<EventRecord>::Iterator event_iter_t;

class EventLists : private Uncopyable
{
public:
    EventLists() = delete;

    EventLists(const EventBufferConfig& config);

    // master list keeps the aggregate order and generic data
    List<EventRecord> events;

    template<class T> List<TypedEventRecord<T>>& GetList();

    bool IsAnyTypeFull() const;

    EventClassCounters counters;

private:
    // sub-lists just act as type-specific storage
    List<TypedEventRecord<BinarySpec>> binary;
    List<TypedEventRecord<DoubleBitBinarySpec>> doubleBinary;
    List<TypedEventRecord<AnalogSpec>> analog;
    List<TypedEventRecord<CounterSpec>> counter;
    List<TypedEventRecord<FrozenCounterSpec>> frozenCounter;
    List<TypedEventRecord<BinaryOutputStatusSpec>> binaryOutputStatus;
    List<TypedEventRecord<AnalogOutputStatusSpec>> analogOutputStatus;
    List<TypedEventRecord<OctetStringSpec>> octetString;
};

} // namespace opendnp3

#endif
