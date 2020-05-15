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
#ifndef OPENDNP3_SERIALIOHANDLER_H
#define OPENDNP3_SERIALIOHANDLER_H

#include "channel/IOHandler.h"
#include "channel/SerialChannel.h"

#include "opendnp3/channel/ChannelRetry.h"
#include "opendnp3/channel/IPEndpoint.h"
#include "opendnp3/channel/SerialSettings.h"

#include <exe4cpp/Timer.h>

namespace opendnp3
{

class SerialIOHandler final : public IOHandler
{

public:
    static std::shared_ptr<SerialIOHandler> Create(const Logger& logger,
                                                   const std::shared_ptr<IChannelListener>& listener,
                                                   const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                                   const ChannelRetry& retry,
                                                   const SerialSettings& settings)
    {
        return std::make_shared<SerialIOHandler>(logger, listener, executor, retry, settings);
    }

    SerialIOHandler(const Logger& logger,
                    const std::shared_ptr<IChannelListener>& listener,
                    const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                    const ChannelRetry& retry,
                    SerialSettings settings);

protected:
    virtual void ShutdownImpl() override;
    virtual void BeginChannelAccept() override;
    virtual void SuspendChannelAccept() override;
    virtual void OnChannelShutdown() override;

private:
    void TryOpen(const TimeDuration& timeout);

    void ResetState();

    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    const ChannelRetry retry;
    const SerialSettings settings;

    // connection retry timer
    exe4cpp::Timer retrytimer;
};

} // namespace opendnp3

#endif
