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

#include "opendnp3/app/AnalogCommandEvent.h"

namespace opendnp3
{

AnalogCommandEvent::AnalogCommandEvent() : value(0), status(CommandStatus::SUCCESS) {}

AnalogCommandEvent::AnalogCommandEvent(double value_, CommandStatus status_) : value(value_), status(status_) {}

AnalogCommandEvent::AnalogCommandEvent(double value_, CommandStatus status_, DNPTime time_)
    : value(value_), status(status_), time(time_)
{
}

bool AnalogCommandEvent::operator==(const AnalogCommandEvent& rhs) const
{
    return (value == rhs.value) && (status == rhs.status) && (time == rhs.time);
}

} // namespace opendnp3
