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

#ifndef OPENDNP3_ERRORCODES_H
#define OPENDNP3_ERRORCODES_H

#include <stdexcept>
#include <string>
#include <system_error>

namespace opendnp3
{

enum class Error : int
{
    SHUTTING_DOWN,
    NO_TLS_SUPPORT,
    UNABLE_TO_BIND_SERVER
};

struct ErrorSpec
{
    static std::string to_string(Error err)
    {
        switch (err)
        {
        case Error::SHUTTING_DOWN:
            return "The operation was requested while the resource was shutting down";
        case Error::NO_TLS_SUPPORT:
            return "Not built with TLS support";
        case Error::UNABLE_TO_BIND_SERVER:
            return "Unable to bind server to the specified port";
        default:
            return "unknown error";
        };
    }
};

class DNP3Error final : public std::runtime_error
{
public:
    explicit DNP3Error(Error err) : std::runtime_error(ErrorSpec::to_string(err)) {}

    DNP3Error(Error err, std::error_code& ec) : std::runtime_error(ErrorSpec::to_string(err) + ": " + ec.message()) {}
};

} // namespace opendnp3

#endif
