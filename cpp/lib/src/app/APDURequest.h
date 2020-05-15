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
#ifndef OPENDNP3_APDUREQUEST_H
#define OPENDNP3_APDUREQUEST_H

#include "APDUWrapper.h"
#include "HeaderWriter.h"

namespace opendnp3
{

// this is what the application layer sees from the master / outstation for transmission
class APDURequest : public APDUWrapper
{
public:
    explicit APDURequest(const ser4cpp::wseq_t& buffer);

    void ConfigureHeader(FunctionCode code, uint8_t seq);

    static APDUEquality Compare(const ser4cpp::rseq_t& lhs, const ser4cpp::rseq_t& rhs);

private:
    APDURequest();
};

} // namespace opendnp3

#endif
