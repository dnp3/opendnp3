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

#include "channel/SerialIOHandler.h"

#include "logging/LogMacros.h"

#include "opendnp3/logging/LogLevels.h"

#include <utility>

namespace opendnp3
{

SerialIOHandler::SerialIOHandler(const Logger& logger,
                                 const std::shared_ptr<IChannelListener>& listener,
                                 const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                                 const ChannelRetry& retry,
                                 SerialSettings settings)
    : IOHandler(logger, false, listener), executor(executor), retry(retry), settings(std::move(settings))
{
}

void SerialIOHandler::ShutdownImpl()
{
    this->ResetState();
}

void SerialIOHandler::BeginChannelAccept()
{
    this->TryOpen(retry.minOpenRetry);
}

void SerialIOHandler::SuspendChannelAccept()
{
    this->ResetState();
}

void SerialIOHandler::OnChannelShutdown()
{
    this->retrytimer = this->executor->start(this->retry.reconnectDelay.value,
                                             [this, self = shared_from_this()]() { this->BeginChannelAccept(); });
}

void SerialIOHandler::TryOpen(const TimeDuration& timeout)
{
    auto port = std::make_shared<SerialChannel>(executor);

    std::error_code ec;
    port->Open(settings, ec);

    if (ec)
    {
        FORMAT_LOG_BLOCK(this->logger, flags::WARN, "Error Connecting: %s", ec.message().c_str());

        ++this->statistics.numOpenFail;

        auto callback = [this, timeout, self = shared_from_this()]() { this->TryOpen(this->retry.NextDelay(timeout)); };

        this->retrytimer = this->executor->start(timeout.value, callback);
    }
    else
    {
        this->OnNewChannel(port);
    }
}

void SerialIOHandler::ResetState()
{
    retrytimer.cancel();
}

} // namespace opendnp3
