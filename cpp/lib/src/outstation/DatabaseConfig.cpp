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

#include "opendnp3/outstation/DatabaseConfig.h"

namespace opendnp3
{

template<class T> void initialize(std::map<uint16_t, T>& map, uint16_t count)
{
    for (uint16_t i = 0; i < count; ++i)
    {
        map[i] = {};
    }
}

DatabaseConfig::DatabaseConfig(uint16_t all_types)
{
    initialize(this->binary_input, all_types);
    initialize(this->double_binary, all_types);
    initialize(this->analog_input, all_types);
    initialize(this->counter, all_types);
    initialize(this->frozen_counter, all_types);
    initialize(this->binary_output_status, all_types);
    initialize(this->analog_output_status, all_types);
    initialize(this->time_and_interval, all_types);
    initialize(this->octet_string, all_types);
};

} // namespace opendnp3
