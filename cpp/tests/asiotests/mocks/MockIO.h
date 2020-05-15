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

#ifndef OPENDNP3_ASIOTESTS_MOCKIO_H
#define OPENDNP3_ASIOTESTS_MOCKIO_H

#include <exe4cpp/asio/StrandExecutor.h>

#include <memory>

class MockIO final : public std::enable_shared_from_this<MockIO>
{
    class Timeout
    {

    public:
        Timeout(asio::io_context& service, std::chrono::steady_clock::duration timeout);

        ~Timeout();

    private:
        std::shared_ptr<asio::basic_waitable_timer<std::chrono::steady_clock>> timer;
    };

public:
    MockIO() : io(std::make_shared<asio::io_context>()) {}

    static std::shared_ptr<MockIO> Create()
    {
        return std::make_shared<MockIO>();
    }

    std::shared_ptr<exe4cpp::StrandExecutor> GetExecutor()
    {
        return exe4cpp::StrandExecutor::create(this->io);
    }

    size_t RunUntilTimeout(const std::function<bool()>& condition,
                           std::chrono::steady_clock::duration timeout = std::chrono::seconds(1));

    void CompleteInMaxXIterations(size_t iterations,
                                  const std::function<bool()>& condition,
                                  std::chrono::steady_clock::duration timeout = std::chrono::seconds(1));

    size_t RunUntilOutOfWork();

    std::shared_ptr<asio::io_context> io;
};

#endif
