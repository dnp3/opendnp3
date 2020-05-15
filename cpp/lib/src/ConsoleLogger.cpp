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
#include "opendnp3/ConsoleLogger.h"

#include "logging/ConsolePrettyPrinter.h"

#include "opendnp3/logging/LogLevels.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <sstream>

namespace opendnp3
{

void ConsoleLogger::log(ModuleId module, const char* id, LogLevel level, char const* location, char const* message)
{
    auto time = std::chrono::high_resolution_clock::now();
    auto num = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();

    std::ostringstream oss;

    oss << "ms(" << num << ") " << LogFlagToString(level);
    oss << " " << id;
    if (printLocation)
    {
        oss << " - " << location;
    }
    oss << " - " << message;

    std::unique_lock<std::mutex> lock(mutex);
    std::cout << oss.str() << std::endl;
}

} // namespace opendnp3
