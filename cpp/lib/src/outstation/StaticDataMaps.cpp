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

#include "StaticDataMaps.h"

namespace opendnp3
{

StaticDataMaps::StaticDataMaps(const DatabaseConfigNew& config)
    : binary_input(config.binary_input),
      double_binary(config.double_binary),
      analog_input(config.analog_input),
      counter(config.counter),
      frozen_counter(config.frozen_counter),
      binary_output_status(config.binary_output_status),
      analog_output_status(config.analog_output_status),
      time_and_interval(config.time_and_interval),
      octet_string(config.octet_string)
{
}

bool StaticDataMaps::has_any_selection() const
{
    return 
		binary_input.has_any_selection() 
		|| double_binary.has_any_selection() 
		|| analog_input.has_any_selection()
        || counter.has_any_selection()
		|| frozen_counter.has_any_selection()
		|| binary_output_status.has_any_selection()
        || analog_output_status.has_any_selection()
		|| time_and_interval.has_any_selection()
        || octet_string.has_any_selection();
}

} // namespace opendnp3
