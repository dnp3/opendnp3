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
#include "opendnp3/outstation/EventBufferConfig.h"

namespace opendnp3
{

EventBufferConfig EventBufferConfig::AllTypes(uint16_t sizes)
{
    return EventBufferConfig(sizes, sizes, sizes, sizes, sizes, sizes, sizes, sizes);
}

EventBufferConfig::EventBufferConfig(uint16_t maxBinaryEvents,
                                     uint16_t maxDoubleBinaryEvents,
                                     uint16_t maxAnalogEvents,
                                     uint16_t maxCounterEvents,
                                     uint16_t maxFrozenCounterEvents,
                                     uint16_t maxBinaryOutputStatusEvents,
                                     uint16_t maxAnalogOutputStatusEvents,
                                     uint16_t maxOctetStringEvents)
    :

      maxBinaryEvents(maxBinaryEvents),
      maxDoubleBinaryEvents(maxDoubleBinaryEvents),
      maxAnalogEvents(maxAnalogEvents),
      maxCounterEvents(maxCounterEvents),
      maxFrozenCounterEvents(maxFrozenCounterEvents),
      maxBinaryOutputStatusEvents(maxBinaryOutputStatusEvents),
      maxAnalogOutputStatusEvents(maxAnalogOutputStatusEvents),
      maxOctetStringEvents(maxOctetStringEvents)
{
}

uint32_t EventBufferConfig::TotalEvents() const
{
    return maxBinaryEvents + maxDoubleBinaryEvents + maxAnalogEvents + maxCounterEvents + maxFrozenCounterEvents
        + maxBinaryOutputStatusEvents + maxAnalogOutputStatusEvents + maxOctetStringEvents;
}

} // namespace opendnp3
