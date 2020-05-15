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
#ifndef OPENDNP3_EVENTCOLLECTION_H
#define OPENDNP3_EVENTCOLLECTION_H

#include "EventWriting.h"
#include "IEventWriteHandler.h"
#include "TypedStorage.h"

namespace opendnp3
{

template<class T> class EventCollection final : public IEventCollection<typename T::meas_t>
{
private:
    List<EventRecord>::Iterator& iterator;
    EventClassCounters& counters;
    typename T::event_variation_t variation;

public:
    EventCollection(List<EventRecord>::Iterator& iterator,
                    EventClassCounters& counters,
                    typename T::event_variation_t variation)
        : iterator(iterator), counters(counters), variation(variation)
    {
    }

    virtual uint16_t WriteSome(IEventWriter<typename T::meas_t>& writer) override;

private:
    bool WriteOne(IEventWriter<typename T::meas_t>& writer);
};

template<class T> uint16_t EventCollection<T>::WriteSome(IEventWriter<typename T::meas_t>& writer)
{
    uint16_t num_written = 0;
    while (WriteOne(writer))
    {
        ++num_written;
    }
    return num_written;
}

template<class T> bool EventCollection<T>::WriteOne(IEventWriter<typename T::meas_t>& writer)
{
    // don't bother searching
    if (this->counters.selected == 0)
        return false;

    // find the next event with the same type and variation
    EventRecord* record = EventWriting::FindNextSelected(this->iterator, T::EventTypeEnum);

    // nothing left to write
    if (!record)
        return false;

    const auto data = TypedStorage<T>::Retrieve(*record);

    // wrong variation
    if (data->value.selectedVariation != this->variation)
        return false;

    // unable to write
    if (!writer.Write(data->value.value, record->index))
        return false;

    // success!
    this->counters.OnWrite(record->clazz);
    record->state = EventState::written;
    this->iterator.Next();
    return true;
}

} // namespace opendnp3

#endif
