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
#ifndef ASIODNP3_CONSOLELOGGER_H
#define ASIODNP3_CONSOLELOGGER_H

#include <openpal/logging/ILogHandler.h>
#include <openpal/util/Uncopyable.h>

#include <memory>
#include <mutex>

namespace asiodnp3
{

std::ostringstream& operator<<(std::ostringstream& ss, const openpal::LogFilters& filters);

/**
 * LogHandler that prints all log messages to the console
 */
class ConsoleLogger final : public openpal::ILogHandler, private openpal::Uncopyable
{

public:
    virtual void Log(const openpal::LogEntry& entry) override;

    static std::shared_ptr<openpal::ILogHandler> Create(bool printLocation = false)
    {
        return std::make_shared<ConsoleLogger>(printLocation);
    };

    ConsoleLogger(bool printLocation) : printLocation(printLocation) {}

private:
    bool printLocation;

    std::mutex mutex;
};

} // namespace asiodnp3

#endif
