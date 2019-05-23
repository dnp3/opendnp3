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
#ifndef ASIOPAL_SERIALIOHANDLER_H
#define ASIOPAL_SERIALIOHANDLER_H

#include "openpal/executor/TimerRef.h"

#include "asiopal/ChannelRetry.h"
#include "asiopal/IPEndpoint.h"
#include "asiopal/SerialChannel.h"
#include "asiopal/SerialTypes.h"

#include "asiodnp3/IOHandler.h"

namespace asiodnp3
{

class SerialIOHandler final : public IOHandler
{

public:
    static std::shared_ptr<SerialIOHandler> Create(const openpal::Logger& logger,
                                                   const std::shared_ptr<IChannelListener>& listener,
                                                   const std::shared_ptr<asiopal::Executor>& executor,
                                                   const asiopal::ChannelRetry& retry,
                                                   const asiopal::SerialSettings& settings)
    {
        return std::make_shared<SerialIOHandler>(logger, listener, executor, retry, settings);
    }

    SerialIOHandler(const openpal::Logger& logger,
                    const std::shared_ptr<IChannelListener>& listener,
                    const std::shared_ptr<asiopal::Executor>& executor,
                    const asiopal::ChannelRetry& retry,
                    asiopal::SerialSettings settings);

protected:
    virtual void ShutdownImpl() override;
    virtual void BeginChannelAccept() override;
    virtual void SuspendChannelAccept() override;
    virtual void OnChannelShutdown() override;

private:
    void TryOpen(const openpal::TimeDuration& timeout);

    void ResetState();

    const std::shared_ptr<asiopal::Executor> executor;
    const asiopal::ChannelRetry retry;
    const asiopal::SerialSettings settings;

    // connection retry timer
    openpal::TimerRef retrytimer;
};

} // namespace asiodnp3

#endif
