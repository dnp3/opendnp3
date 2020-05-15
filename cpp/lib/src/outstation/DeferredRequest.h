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
#ifndef OPENDNP3_DEFERREDREQUEST_H
#define OPENDNP3_DEFERREDREQUEST_H

#include "ParsedRequest.h"

#include "opendnp3/util/Uncopyable.h"

#include <ser4cpp/container/Buffer.h>

namespace opendnp3
{

/**
 * Records metadata about deferred requests
 */
class DeferredRequest : private Uncopyable
{

public:
    explicit DeferredRequest(uint32_t maxAPDUSize);

    void Reset();

    bool IsSet() const;

    FunctionCode GetFunction() const;

    void Set(const ParsedRequest& request);

    template<class Handler> bool Process(const Handler& handler);

private:
    DeferredRequest() = delete;

    bool isSet;
    Addresses addresses;
    APDUHeader header;
    ser4cpp::rseq_t objects;
    ser4cpp::Buffer buffer;
};

template<class Handler> bool DeferredRequest::Process(const Handler& handler)
{
    if (isSet)
    {
        bool processed = handler(ParsedRequest(this->addresses, this->header, this->objects));
        isSet = !processed;
        return processed;
    }
    else
    {
        return false;
    }
}

} // namespace opendnp3

#endif
