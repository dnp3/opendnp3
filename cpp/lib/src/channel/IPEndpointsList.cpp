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
#include "channel/IPEndpointsList.h"

#include <cassert>

namespace opendnp3
{

IPEndpointsList::IPEndpointsList(const std::vector<IPEndpoint>& endpoints)
    : endpoints(endpoints), currentEndpoint(this->endpoints.begin())
{
    assert(!endpoints.empty());
}

IPEndpointsList::IPEndpointsList(const IPEndpointsList& rhs)
    : endpoints(rhs.endpoints), currentEndpoint(this->endpoints.begin())
{
}

const IPEndpoint& IPEndpointsList::GetCurrentEndpoint()
{
    return *this->currentEndpoint;
}

void IPEndpointsList::Next()
{
    ++this->currentEndpoint;
    if (this->currentEndpoint == this->endpoints.end())
    {
        Reset();
    }
}

void IPEndpointsList::Reset()
{
    this->currentEndpoint = this->endpoints.begin();
}

} // namespace opendnp3
