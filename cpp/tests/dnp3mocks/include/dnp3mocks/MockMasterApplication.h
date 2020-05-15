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
#ifndef OPENDNP3_UNITTESTS_MOCK_MASTER_APPLICATION_H
#define OPENDNP3_UNITTESTS_MOCK_MASTER_APPLICATION_H

#include <opendnp3/master/IMasterApplication.h>

#include <deque>
#include <functional>
#include <vector>

class MockMasterApplication final : public opendnp3::IMasterApplication
{

public:
    enum class State
    {
        OPEN,
        CLOSED
    };

    MockMasterApplication() : time(0) {}

    virtual opendnp3::UTCTimestamp Now() override
    {
        return opendnp3::UTCTimestamp(time);
    }

    virtual void OnReceiveIIN(const opendnp3::IINField& iin) override
    {
        rxIIN.push_back(iin);
    }

    virtual void OnTaskStart(opendnp3::MasterTaskType type, opendnp3::TaskId id) override
    {
        taskStartEvents.push_back(type);
    }

    virtual void OnTaskComplete(const opendnp3::TaskInfo& info) override
    {
        taskCompletionEvents.push_back(info);
    }

    virtual void OnOpen() override
    {
        this->stateChanges.push_back(State::OPEN);
    }

    virtual void OnClose() override
    {
        this->stateChanges.push_back(State::CLOSED);
    }

    virtual bool AssignClassDuringStartup() override
    {
        return !assignClassHeaders.empty();
    }

    virtual void ConfigureAssignClassRequest(const opendnp3::WriteHeaderFunT& fun) override
    {
        for (auto& header : assignClassHeaders)
        {
            fun(header);
        }
    }

    std::deque<State> stateChanges;

    std::vector<opendnp3::Header> assignClassHeaders;

    std::vector<opendnp3::IINField> rxIIN;

    std::vector<opendnp3::MasterTaskType> taskStartEvents;
    std::vector<opendnp3::TaskInfo> taskCompletionEvents;

    uint64_t time;
};

#endif
