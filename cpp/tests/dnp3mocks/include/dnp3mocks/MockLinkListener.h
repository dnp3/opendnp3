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
#ifndef OPENDNP3_UNITTESTS_MOCK_LINK_LISTENER_H_
#define OPENDNP3_UNITTESTS_MOCK_LINK_LISTENER_H_

#include <opendnp3/link/ILinkListener.h>

#include <vector>

class MockLinkListener final : public opendnp3::ILinkListener
{
public:
    MockLinkListener() : numKeepAliveTransmissions(0), numKeepAliveFailure(0), numKeepAliveReplys(0) {}

    virtual void OnStateChange(opendnp3::LinkStatus value) override
    {
        statusValues.push_back(value);
    }

    virtual void OnKeepAliveInitiated() override
    {
        ++numKeepAliveTransmissions;
    }

    virtual void OnKeepAliveFailure() override
    {
        ++numKeepAliveFailure;
    }

    virtual void OnKeepAliveSuccess() override
    {
        ++numKeepAliveReplys;
    }

    uint32_t numKeepAliveTransmissions;
    uint32_t numKeepAliveFailure;
    uint32_t numKeepAliveReplys;

    std::vector<opendnp3::LinkStatus> statusValues;
};

#endif
