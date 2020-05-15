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
#include "logging/LogMacros.h"

#include <opendnp3/logging/LogLevels.h>

#include "dnp3mocks/MockLogHandler.h"

#include <catch.hpp>

#include <iostream>
#include <vector>

using namespace opendnp3;

#define SUITE(name) "LogTest - " name

TEST_CASE(SUITE("FORMAT_SAFE macro truncates and null terminates"))
{
    char buffer[10];
    auto text1 = "hello";
    auto text2 = "my little friend";

    SAFE_STRING_FORMAT(buffer, 10, "%s %s", text1, text2);

    std::string result(buffer);

    REQUIRE(result.size() == 9);

    REQUIRE(result == "hello my ");
}
