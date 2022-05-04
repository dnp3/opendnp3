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
#ifndef OPENDNP3_UDPSOCKETCHANNEL_H
#define OPENDNP3_UDPSOCKETCHANNEL_H

#include "channel/IAsyncChannel.h"
#include "opendnp3/logging/Logger.h"

namespace opendnp3
{

class UDPSocketChannel final : public IAsyncChannel
{

public:
    static std::shared_ptr<IAsyncChannel> Create(std::shared_ptr<exe4cpp::StrandExecutor> executor,
                                                 const Logger& logger, asio::ip::udp::socket socket)
    {
        return std::make_shared<UDPSocketChannel>(executor, logger, std::move(socket));
    }

    UDPSocketChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor, const Logger& logger, asio::ip::udp::socket socket);

protected:
    void BeginReadImpl(ser4cpp::wseq_t dest) final;
    void BeginWriteImpl(const ser4cpp::rseq_t& buffer) final;
    void ShutdownImpl() final;

private:
    Logger logger;
    asio::ip::udp::socket socket;
    bool first_successful_read;
    uint8_t num_first_read_retries;
};

} // namespace opendnp3

#endif
