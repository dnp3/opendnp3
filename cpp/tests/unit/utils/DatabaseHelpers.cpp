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

#include "DatabaseHelpers.h"

namespace configure
{

namespace by_count_of
{
    opendnp3::DatabaseConfigNew all_types(uint16_t num)
    {
        return database_by_sizes(num, num, num, num, num, num, num, num, num);
    }

    opendnp3::DatabaseConfigNew binary_input(uint16_t num)
    {
        return database_by_sizes(num, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    opendnp3::DatabaseConfigNew counter(uint16_t num)
    {
        return database_by_sizes(0, 0, 0, num, 0, 0, 0, 0, 0);
    }

    opendnp3::DatabaseConfigNew binary_output_status(uint16_t num)
    {
        return database_by_sizes(0, 0, 0, 0, 0, num, 0, 0, 0);
    }

    opendnp3::DatabaseConfigNew analog_input(uint16_t num)
    {
        return database_by_sizes(0, 0, num, 0, 0, 0, 0, 0, 0);
    }

    opendnp3::DatabaseConfigNew analog_output_status(uint16_t num)
	{
        return database_by_sizes(0, 0, 0, 0, 0, 0, num, 0, 0);
	}

    opendnp3::DatabaseConfigNew time_and_interval(uint16_t num)
    {
        return database_by_sizes(0, 0, 0, 0, 0, 0, 0, num, 0);
    }

    opendnp3::DatabaseConfigNew octet_string(uint16_t num)
    {
        return database_by_sizes(0, 0, 0, 0, 0, 0, 0, 0, num);
    }

} // namespace by_count_of

opendnp3::DatabaseConfigNew from(std::map<uint16_t, opendnp3::AnalogConfig> map)
{
    opendnp3::DatabaseConfigNew config;
    config.analog_input = std::move(map);
    return config;
}

opendnp3::DatabaseConfigNew from(std::map<uint16_t, opendnp3::BinaryConfig> map)
{
    opendnp3::DatabaseConfigNew config;
    config.binary_input = std::move(map);
    return config;
}

opendnp3::AnalogConfig analog(opendnp3::StaticAnalogVariation variation)
{
    opendnp3::AnalogConfig config;
    config.svariation = variation;
    return config;
}

opendnp3::DatabaseConfigNew database_by_sizes(uint16_t num_binary,
                                              uint16_t num_double_binary,
                                              uint16_t num_analog,
                                              uint16_t num_counter,
                                              uint16_t num_frozen_counter,
                                              uint16_t num_binary_output_status,
                                              uint16_t num_analog_output_status,
                                              uint16_t num_time_and_interval,
                                              uint16_t num_octet_string)
{
    opendnp3::DatabaseConfigNew config;

    for (uint16_t i = 0; i < num_binary; ++i)
    {
        config.binary_input[i] = {};
    }
    for (uint16_t i = 0; i < num_double_binary; ++i)
    {
        config.double_binary[i] = {};
    }
    for (uint16_t i = 0; i < num_analog; ++i)
    {
        config.analog_input[i] = {};
    }
    for (uint16_t i = 0; i < num_counter; ++i)
    {
        config.counter[i] = {};
    }
    for (uint16_t i = 0; i < num_frozen_counter; ++i)
    {
        config.frozen_counter[i] = {};
    }
    for (uint16_t i = 0; i < num_binary_output_status; ++i)
    {
        config.binary_output_status[i] = {};
    }
    for (uint16_t i = 0; i < num_analog_output_status; ++i)
    {
        config.analog_output_status[i] = {};
    }
    for (uint16_t i = 0; i < num_time_and_interval; ++i)
    {
        config.time_and_interval[i] = {};
    }
    for (uint16_t i = 0; i < num_octet_string; ++i)
    {
        config.octet_string[i] = {};
    }

    return config;
}

} // namespace configure
