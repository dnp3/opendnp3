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
#ifndef OPENDNP3_IVARIABLELENGTH_H
#define OPENDNP3_IVARIABLELENGTH_H

#include "opendnp3/app/GroupVariationID.h"

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

class IVariableLength
{

public:
    virtual ~IVariableLength() {}

    virtual GroupVariationID InstanceID() const = 0;

    // The size of the object in its current configuration
    virtual size_t Size() const = 0;

    // attempt to write the object to the destination buffer
    virtual bool Write(ser4cpp::wseq_t& dest) const = 0;

    // attempt to read the object from the input buffer
    virtual bool Read(const ser4cpp::rseq_t& input) = 0;
};

} // namespace opendnp3

#endif
