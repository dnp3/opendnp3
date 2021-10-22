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
#ifndef OPENDNP3_EVENTBUFFER_H
#define OPENDNP3_EVENTBUFFER_H

#include "EventStorage.h"
#include "outstation/IEventReceiver.h"
#include "outstation/IEventSelector.h"
#include "outstation/IResponseLoader.h"

#include "opendnp3/app/ClassField.h"
#include "opendnp3/outstation/EventBufferConfig.h"

namespace opendnp3
{

/*
    The sequence of events list is a doubly linked-list implemented
    in a finite array.  The list is desired for O(1) remove operations from
    arbitrary parts of the list depending on what the user asks for in terms
    of event type or Class1/2/3.

    At worst, selection is O(n) but it has some type/class tracking to avoid looping
    over the SOE list when there are no more events to be written.
*/

class EventBuffer final : public IEventReceiver, public IEventSelector, public IResponseLoader
{

public:
    explicit EventBuffer(const EventBufferConfig& config);

    // ------- IEventReceiver ------

    virtual void Update(const Event<BinarySpec>& evt) override;
    virtual void Update(const Event<DoubleBitBinarySpec>& evt) override;
    virtual void Update(const Event<AnalogSpec>& evt) override;
    virtual void Update(const Event<CounterSpec>& evt) override;
    virtual void Update(const Event<FrozenCounterSpec>& evt) override;
    virtual void Update(const Event<BinaryOutputStatusSpec>& evt) override;
    virtual void Update(const Event<AnalogOutputStatusSpec>& evt) override;
    virtual void Update(const Event<OctetStringSpec>& evt) override;

    // ------- IEventSelector ------

    virtual void Unselect();

    virtual IINField SelectAll(GroupVariation gv) override final;

    virtual IINField SelectCount(GroupVariation gv, uint16_t count) override final;

    // ------- IResponseLoader -------

    virtual bool HasAnySelection() const override final;

    virtual bool Load(HeaderWriter& writer) override final;

    // ------- Misc -------

    void ClearWritten(); // called when a transmission succeeds

    ClassField UnwrittenClassField() const;

    bool IsOverflown();

    void SelectAllByClass(const ClassField& clazz);

    uint32_t NumEvents(EventClass ec) const;
    

private:
    bool overflow = false;
    EventStorage storage;

    IINField SelectMaxCount(GroupVariation gv, uint32_t maximum);

    template<class T> IINField SelectByType(uint32_t max, T type)
    {
        this->storage.SelectByType(type, max);
        return IINField::Empty();
    }

    template<class T> void UpdateAny(const Event<T>& evt)
    {
        if (this->storage.Update(evt))
        {
            this->overflow = true;
        }
    }

    IINField SelectByClass(uint32_t max, EventClass clazz)
    {
        this->storage.SelectByClass(clazz, max);
        return IINField::Empty();
    }
};

} // namespace opendnp3

#endif
