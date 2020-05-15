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
#ifndef OPENDNP3_DATABASECONFIG_H
#define OPENDNP3_DATABASECONFIG_H

#include "opendnp3/outstation/MeasurementConfig.h"

#include <map>

namespace opendnp3
{

struct DatabaseConfig
{
    DatabaseConfig() = default;

    DatabaseConfig(uint16_t all_types);

    std::map<uint16_t, BinaryConfig> binary_input;
    std::map<uint16_t, DoubleBitBinaryConfig> double_binary;
    std::map<uint16_t, AnalogConfig> analog_input;
    std::map<uint16_t, CounterConfig> counter;
    std::map<uint16_t, FrozenCounterConfig> frozen_counter;
    std::map<uint16_t, BOStatusConfig> binary_output_status;
    std::map<uint16_t, AOStatusConfig> analog_output_status;
    std::map<uint16_t, TimeAndIntervalConfig> time_and_interval;
    std::map<uint16_t, OctetStringConfig> octet_string;
};

} // namespace opendnp3

#endif
