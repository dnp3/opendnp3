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
#ifndef ASIOPAL_TLSSTREAMCHANNEL_H
#define ASIOPAL_TLSSTREAMCHANNEL_H

#include "asiopal/IAsyncChannel.h"

#include <asio/ssl.hpp>

namespace asiopal
{

class TLSStreamChannel final : public IAsyncChannel
{

public:
    static std::shared_ptr<IAsyncChannel> Create(
        const std::shared_ptr<Executor>& executor,
        const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>>& stream)
    {
        return std::make_shared<TLSStreamChannel>(executor, stream);
    }

    TLSStreamChannel(const std::shared_ptr<Executor>& executor,
                     std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream);

private:
    virtual void BeginReadImpl(openpal::WSlice dest) override;
    virtual void BeginWriteImpl(const openpal::RSlice& data) override;
    virtual void ShutdownImpl() override;

    const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream;
};

} // namespace asiopal

#endif
