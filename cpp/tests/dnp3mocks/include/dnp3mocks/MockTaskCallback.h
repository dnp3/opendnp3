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
#ifndef OPENDNP3_UNITTESTS_MOCK_COMMAND_CALLBACK_H
#define OPENDNP3_UNITTESTS_MOCK_COMMAND_CALLBACK_H

#include <opendnp3/master/ITaskCallback.h>

#include <queue>

class MockTaskCallback : public opendnp3::ITaskCallback
{
public:
    void OnStart() final
    {
        ++numStart;
    }

    void OnComplete(opendnp3::TaskCompletion result) final
    {
        results.push_back(result);
    }

    void OnDestroyed() final
    {
        ++numDestroyed;
    }

    uint32_t numStart = 0;

    uint32_t numDestroyed = 0;

    std::deque<opendnp3::TaskCompletion> results;
};

#endif
