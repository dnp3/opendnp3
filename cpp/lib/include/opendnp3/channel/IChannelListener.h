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
#ifndef OPENDNP3_ICHANNELLISTENER_H
#define OPENDNP3_ICHANNELLISTENER_H

#include "opendnp3/gen/ChannelState.h"

namespace opendnp3
{

/**
 * Callback interface for receiving information about a running channel
 */
class IChannelListener
{
public:
    virtual ~IChannelListener() {}

    /*
     * Receive callbacks for state transitions on the channels executor
     */
    virtual void OnStateChange(ChannelState state) = 0;
};

} // namespace opendnp3

#endif
