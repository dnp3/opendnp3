/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "opendnp3/channel/ChannelRetry.h"

namespace opendnp3
{

ChannelRetry::ChannelRetry(TimeDuration minOpenRetry_,
                           TimeDuration maxOpenRetry_,
                           TimeDuration reconnectDelay_,
                           IOpenDelayStrategy& strategy_)
    : minOpenRetry(minOpenRetry_), maxOpenRetry(maxOpenRetry_), reconnectDelay(reconnectDelay_), strategy(strategy_)
{
}

ChannelRetry ChannelRetry::Default()
{
    return ChannelRetry(TimeDuration::Seconds(1), TimeDuration::Minutes(1));
}

} // namespace opendnp3
