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
#ifndef OPENDNP3_LOGGER_H
#define OPENDNP3_LOGGER_H

#include "opendnp3/logging/ILogHandler.h"

#include <memory>
#include <string>

namespace opendnp3
{

const uint32_t max_log_entry_size = 120;

/**
 * A copyable facade over a LogRoot class
 */
class Logger
{
public:
    struct Settings
    {
        Settings(ModuleId module, const std::string& id, LogLevels levels) : module(module), id(id), levels(levels) {}

        ModuleId module;
        std::string id;
        LogLevels levels;
    };

    Logger(const std::shared_ptr<ILogHandler>& backend, ModuleId moduleid, const std::string& id, LogLevels levels)
        : backend(backend), settings(std::make_shared<Settings>(moduleid, id, levels))
    {
    }

    static Logger empty()
    {
        return Logger(nullptr, ModuleId(0), "", LogLevels(0));
    }

    void log(const LogLevel& level, const char* location, const char* message)
    {
        if (backend)
        {
            backend->log(this->settings->module, this->settings->id.c_str(), level, location, message);
        }
    }

    Logger detach(const std::string& id) const
    {
        return Logger(this->backend, std::make_shared<Settings>(this->settings->module, id, this->settings->levels));
    }

    Logger detach(const std::string& id, LogLevels levels) const
    {
        return Logger(this->backend, std::make_shared<Settings>(this->settings->module, id, levels));
    }

    Logger detach(LogLevels levels) const
    {
        return Logger(this->backend, std::make_shared<Settings>(this->settings->module, this->settings->id, levels));
    }

    bool is_enabled(const LogLevel& level) const
    {
        return backend && settings->levels.is_set(level);
    }

    LogLevels get_levels() const
    {
        return this->settings->levels;
    }

    void set_levels(const LogLevels& filters)
    {
        this->settings->levels = filters;
    }

    void rename(const std::string& id)
    {
        this->settings->id = id;
    }

private:
    Logger(const std::shared_ptr<ILogHandler>& backend, const std::shared_ptr<Settings>& settings)
        : backend(backend), settings(settings)
    {
    }

    Logger() = delete;
    Logger& operator=(const Logger&) = delete;

    const std::shared_ptr<ILogHandler> backend;
    const std::shared_ptr<Settings> settings;
};

} // namespace opendnp3

#endif // OPENDNP3_LOGGER_H
