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

#include "asiodnp3/PrintingCommandCallback.h"

#include <iostream>

using namespace opendnp3;

namespace asiodnp3
{
opendnp3::CommandCallbackT PrintingCommandCallback::Get()
{
    return [](const ICommandTaskResult& result) -> void {
        std::cout << "Received command result w/ summary: " << TaskCompletionToString(result.summary) << std::endl;
        auto print = [](const CommandPointResult& res) {
            std::cout << "Header: " << res.headerIndex << " Index: " << res.index
                      << " State: " << CommandPointStateToString(res.state)
                      << " Status: " << CommandStatusToString(res.status);
        };
        result.ForeachItem(print);
    };
}
} // namespace asiodnp3
