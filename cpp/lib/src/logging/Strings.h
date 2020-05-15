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
#ifndef OPENDNP3_STRINGS_H
#define OPENDNP3_STRINGS_H

#include <sstream>

namespace opendnp3
{

/**
 * String manipulation
 */
class Strings
{
public:
    // Append anything together that can be used w/ ostringstream
    template<typename... Args> static std::string concatenate(Args... args)
    {
        std::ostringstream oss;
        append(oss, args...);
        return oss.str();
    }

private:
    template<typename T, typename... Args> static void append(std::ostringstream& oss, const T& first, Args... args)
    {
        oss << first;
        append(oss, args...);
    }

    static void append(std::ostringstream& oss) {}
};

} // namespace opendnp3

#endif // OPENDNP3_STRINGS_H
