/*
 * Copyright 2013-2019 Automatak, LLC
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
#ifndef OPENDNP3_UPDATEBUILDER_H
#define OPENDNP3_UPDATEBUILDER_H

#include "opendnp3/outstation/Updates.h"

namespace opendnp3
{

class UpdateBuilder
{

public:
    UpdateBuilder& Update(const Binary& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const Analog& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const Counter& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const FrozenCounter& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const OctetString& meas, uint16_t index, EventMode mode = EventMode::Detect);
    UpdateBuilder& Update(const TimeAndInterval& meas, uint16_t index);
    UpdateBuilder& Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags);

    Updates Build();

private:
    template<class T> UpdateBuilder& AddMeas(const T& meas, uint16_t index, EventMode mode);

    void Add(const update_func_t& fun);

    std::shared_ptr<shared_updates_t> updates;
};

} // namespace opendnp3

#endif
