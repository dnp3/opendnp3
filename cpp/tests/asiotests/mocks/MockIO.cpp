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

#include "mocks/MockIO.h"

#include <sstream>

MockIO::Timeout::Timeout(asio::io_context& service, std::chrono::steady_clock::duration timeout)
    : timer(std::make_shared<asio::basic_waitable_timer<std::chrono::steady_clock>>(service, timeout))
{
    auto callback = [t = timer](const std::error_code& ec) {
        if (!ec)
        {
            throw std::logic_error("timeout before completion");
        }
    };

    this->timer->async_wait(callback);
}

MockIO::Timeout::~Timeout()
{
    this->timer->cancel();
}

size_t MockIO::RunUntilTimeout(const std::function<bool()>& condition, std::chrono::steady_clock::duration timeout)
{
    size_t iterations = 0;
    Timeout to(*this->io, timeout);

    while (!condition())
    {
        std::error_code ec;
        const auto num = this->io->run_one(ec);
        if (ec)
            throw std::logic_error(ec.message());
        if (num == 0)
        {
            std::ostringstream oss;
            oss << "Ran out of work after " << iterations << " iterations";
            throw std::logic_error(oss.str());
        }

        ++iterations;

        this->io.reset();
    }

    return iterations;
}

void MockIO::CompleteInMaxXIterations(size_t expectedIterations,
                                      const std::function<bool()>& condition,
                                      std::chrono::steady_clock::duration timeout)
{
    size_t iterations = 0;

    Timeout to(*this->io, timeout);

    while (!condition())
    {
        if (iterations == expectedIterations)
        {
            std::ostringstream oss;
            oss << "not complete after " << iterations << " iterations";
            throw std::logic_error(oss.str());
        }

        std::error_code ec;
        const auto num = this->io->run_one(ec);
        if (ec)
            throw std::logic_error(ec.message());
        if (num == 0)
        {
            std::ostringstream oss;
            oss << "Ran out of work after " << iterations << " iterations";
            throw std::logic_error(oss.str());
        }

        ++iterations;
        this->io->reset();
    }
}

size_t MockIO::RunUntilOutOfWork()
{
    size_t iterations = 0;

    while (true)
    {
        std::error_code ec;
        const auto num = this->io->poll_one(ec);
        if (ec)
            throw std::logic_error(ec.message());

        if (num == 0)
        {
            return iterations;
        }

        ++iterations;

        this->io->reset();
    }
}
