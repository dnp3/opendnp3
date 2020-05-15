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
#ifndef OPENDNP3_UNITTESTS_DATABASE_HELPERS_H
#define OPENDNP3_UNITTESTS_DATABASE_HELPERS_H

#include <opendnp3/outstation/DatabaseConfig.h>

namespace configure
{

namespace by_count_of
{
    opendnp3::DatabaseConfig all_types(uint16_t num);
    opendnp3::DatabaseConfig binary_input(uint16_t num);
    opendnp3::DatabaseConfig counter(uint16_t num, bool with_frozen = false);
    opendnp3::DatabaseConfig binary_output_status(uint16_t num);
    opendnp3::DatabaseConfig analog_input(uint16_t num);
    opendnp3::DatabaseConfig analog_output_status(uint16_t num);
    opendnp3::DatabaseConfig time_and_interval(uint16_t num);
    opendnp3::DatabaseConfig octet_string(uint16_t num);

} // namespace by_count_of

opendnp3::DatabaseConfig from(std::map<uint16_t, opendnp3::AnalogConfig> map);
opendnp3::DatabaseConfig from(std::map<uint16_t, opendnp3::BinaryConfig> map);

opendnp3::AnalogConfig analog(opendnp3::StaticAnalogVariation variation);

opendnp3::DatabaseConfig database_by_sizes(uint16_t num_binary,
                                           uint16_t num_double_binary,
                                           uint16_t num_analog,
                                           uint16_t num_counter,
                                           uint16_t num_frozen_counter,
                                           uint16_t num_binary_output_status,
                                           uint16_t num_analog_output_status,
                                           uint16_t num_time_and_interval,
                                           uint16_t num_octet_string);

} // namespace configure

#endif
