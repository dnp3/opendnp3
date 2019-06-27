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
#include "opendnp3/outstation/DefaultOutstationApplication.h"

namespace opendnp3
{

DefaultOutstationApplication::DefaultOutstationApplication(TimeDuration timeSyncRefreshRate)
    : refresh_rate(timeSyncRefreshRate)
{

}

DNPTime DefaultOutstationApplication::Now()
{
    auto result = DNPTime(last_timestamp.msSinceEpoch + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_update).count());
    result.quality = IsTimeValid() ? TimestampQuality::SYNCHRONIZED : TimestampQuality::UNSYNCHRONIZED;
    return result;
}

bool DefaultOutstationApplication::WriteAbsoluteTime(const UTCTimestamp& timestamp)
{
    last_timestamp = timestamp;
    last_update = std::chrono::steady_clock::now();
    return true;
}

ApplicationIIN DefaultOutstationApplication::GetApplicationIIN() const
{
    ApplicationIIN result;
    result.needTime = !IsTimeValid();
    return result;
}

bool DefaultOutstationApplication::IsTimeValid() const
{
    return std::chrono::steady_clock::now() - last_update <= refresh_rate.value;
}

} // namespace opendnp3
