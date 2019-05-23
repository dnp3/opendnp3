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
#include "asiopal/IOpenDelayStrategy.h"

namespace asiopal
{

ExponentialBackoffStrategy ExponentialBackoffStrategy::instance;

IOpenDelayStrategy& ExponentialBackoffStrategy::Instance()
{
    return instance;
}

openpal::TimeDuration ExponentialBackoffStrategy::GetNextDelay(const openpal::TimeDuration& current,
                                                               const openpal::TimeDuration& max) const
{
    int64_t doubled = 2 * current.GetMilliseconds();
    return (doubled > max.GetMilliseconds()) ? max : openpal::TimeDuration::Milliseconds(doubled);
}

} // namespace asiopal
