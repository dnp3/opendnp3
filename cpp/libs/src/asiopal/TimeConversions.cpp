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

#include "asiopal/TimeConversions.h"

namespace asiopal
{

steady_clock_t::time_point TimeConversions::Convert(const openpal::MonotonicTimestamp& timestamp)
{
    return (timestamp.milliseconds > MAX_MILLISECONDS)
        ? steady_clock_t::time_point::max()
        : steady_clock_t::time_point(std::chrono::milliseconds(timestamp.milliseconds));
}

openpal::MonotonicTimestamp TimeConversions::Convert(const steady_clock_t::time_point& timestamp)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count();
    return openpal::MonotonicTimestamp(ms);
}

} // namespace asiopal
