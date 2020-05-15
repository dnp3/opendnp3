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

#include "dnp3mocks/MockLogHandler.h"

#include <opendnp3/logging/LogLevels.h>

#include <iostream>

using namespace opendnp3;

LogRecord::LogRecord(ModuleId module, const char* id, LogLevel level, char const* location, char const* message)
    : module(module), id(id), level(level), location(location), message(message)
{
}

void MockLogHandlerImpl::log(ModuleId module, const char* id, LogLevel level, char const* location, char const* message)
{
    std::lock_guard<std::mutex> lock(this->mutex);

    if (outputToStdIO)
    {
        std::cout << id << " - " << message << std::endl;
    }

    this->messages.emplace_back(module, id, level, location, message);
}

void MockLogHandler::ClearLog()
{
    this->impl->messages.clear();
}

void MockLogHandler::Log(const std::string& location, const std::string& message)
{
    this->impl->log(ModuleId(), "test", opendnp3::flags::EVENT, location.c_str(), message.c_str());
}

void MockLogHandler::WriteToStdIo()
{
    this->impl->outputToStdIO = true;
}

bool MockLogHandler::GetNextEntry(LogRecord& record)
{
    if (impl->messages.empty())
        return false;

    record = impl->messages.front();
    impl->messages.pop_front();
    return true;
}
