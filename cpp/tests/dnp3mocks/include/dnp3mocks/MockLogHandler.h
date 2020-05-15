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

#ifndef OPENDNP3_UNITTESTS_MOCKLOGHANDLER_H
#define OPENDNP3_UNITTESTS_MOCKLOGHANDLER_H

#include "opendnp3/logging/Logger.h"

#include <mutex>
#include <queue>
#include <string>

class LogRecord
{
public:
    LogRecord() = default;
    LogRecord(
        opendnp3::ModuleId module, const char* id, opendnp3::LogLevel level, char const* location, char const* message);

    opendnp3::ModuleId module;
    std::string id;
    opendnp3::LogLevel level;
    std::string location;
    std::string message;
};

struct MockLogHandlerImpl : public opendnp3::ILogHandler
{
    virtual void log(opendnp3::ModuleId module,
                     const char* id,
                     opendnp3::LogLevel level,
                     char const* location,
                     char const* message) override;

    std::mutex mutex;
    bool outputToStdIO = false;
    std::deque<LogRecord> messages;
};

class MockLogHandler
{

public:
    MockLogHandler()
        : impl(std::make_shared<MockLogHandlerImpl>()),
          logger(impl, opendnp3::ModuleId(), "test", opendnp3::LogLevels::everything())
    {
    }

    void WriteToStdIo();

    void Log(const std::string& location, const std::string& message);

    void ClearLog();

    bool GetNextEntry(LogRecord& record);

private:
    std::shared_ptr<MockLogHandlerImpl> impl;

public:
    opendnp3::Logger logger;
};

#endif
