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
#ifndef OPENDNP3_CONSOLEPRETTYPRINTER_H
#define OPENDNP3_CONSOLEPRETTYPRINTER_H

#include "opendnp3/logging/ILogHandler.h"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace opendnp3
{

/**
 * pretty prints log messages
 */
class ConsolePrettyPrinter final : public opendnp3::ILogHandler
{
public:
    class Settings
    {
    public:
        Settings() {}

        bool printId = true;
        size_t max_id_size = 10;
    };

    explicit ConsolePrettyPrinter(const Settings& settings = Settings()) : settings(settings) {}

    void log(opendnp3::ModuleId module,
             const char* id,
             opendnp3::LogLevel level,
             char const* location,
             char const* message) override
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        std::ostringstream oss;

        oss << "ms(" << millis << ") ";

        std::string id_str(id);

        if (id_str.size() > settings.max_id_size)
        {
            id_str = id_str.substr(0, settings.max_id_size);
        }
        else if (id_str.size() << settings.max_id_size)
        {
            id_str = id_str + std::string(settings.max_id_size - id_str.size(), ' ');
        }

        if (settings.printId)
        {
            oss << id_str << " ";
        }

        oss << get_prefix(level.value) << message;

        std::cout << oss.str() << std::endl;
    }

private:
    const Settings settings;

    static const char* get_prefix(int level)
    {
        switch (level)
        {
        case (1 << 0):
            return "event   ";
        case (1 << 1):
            return "error   ";
        case (1 << 2):
            return "warn    ";
        case (1 << 3):
            return "info    ";
        case (1 << 4):
            return "debug   ";
        case (1 << 5):
            return "<--     ";
        case (1 << 6):
            return "-->     ";
        default:
            return "            ";
        }
    }
};

} // namespace opendnp3

#endif // OPENDNP3_CONSOLEPRETTYPRINTER_H
