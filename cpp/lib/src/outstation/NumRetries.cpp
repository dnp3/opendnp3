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

#include "opendnp3/outstation/NumRetries.h"

namespace opendnp3
{

NumRetries::NumRetries(std::size_t maxNumRetries, bool isInfinite)
    : numRetries(0), maxNumRetries(maxNumRetries), isInfinite(isInfinite)
{
}

NumRetries NumRetries::Fixed(std::size_t maxNumRetries)
{
    return NumRetries(maxNumRetries, false);
}

NumRetries NumRetries::Infinite()
{
    return NumRetries(0, true);
}

bool NumRetries::Retry()
{
    this->numRetries++;

    return this->isInfinite || this->numRetries <= this->maxNumRetries;
}

void NumRetries::Reset()
{
    this->numRetries = 0;
}

}; // namespace opendnp3
