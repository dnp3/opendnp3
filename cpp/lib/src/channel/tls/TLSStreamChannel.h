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
#ifndef OPENDNP3_TLSSTREAMCHANNEL_H
#define OPENDNP3_TLSSTREAMCHANNEL_H

#include "channel/IAsyncChannel.h"

#include <asio/ssl.hpp>

namespace opendnp3
{

class TLSStreamChannel final : public IAsyncChannel
{

public:
    static std::shared_ptr<IAsyncChannel> Create(
        const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
        const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>>& stream)
    {
        return std::make_shared<TLSStreamChannel>(executor, stream);
    }

    TLSStreamChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
                     std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream);

private:
    void BeginReadImpl(ser4cpp::wseq_t dest) final;
    void BeginWriteImpl(const ser4cpp::rseq_t& data) final;
    void ShutdownImpl() final;

    const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream;
};

} // namespace opendnp3

#endif
