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

#ifndef OPENDNP3_INTEGRATIONTESTS_QUEUEDCHANNELISTENER_H
#define OPENDNP3_INTEGRATIONTESTS_QUEUEDCHANNELISTENER_H

#include "mocks/SynchronizedQueue.h"

#include <opendnp3/channel/IChannelListener.h>

#include <vector>

class QueuedChannelListener : public opendnp3::IChannelListener
{
    SynchronizedQueue<opendnp3::ChannelState> states;

public:
    virtual void OnStateChange(opendnp3::ChannelState state) override
    {
        states.Add(state);
    }

    bool WaitForState(opendnp3::ChannelState state, std::chrono::steady_clock::duration timeout)
    {
        std::vector<opendnp3::ChannelState> output;
        while (states.DrainTo(output, timeout) > 0)
        {
            for (auto& s : output)
            {
                if (s == state)
                    return true;
            }
            output.clear();
        }

        return false;
    }
};

#endif
