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
#ifndef OPENDNP3_DNPTIMEPARSING_H
#define OPENDNP3_DNPTIMEPARSING_H

#include "opendnp3/app/DNPTime.h"

#include <ser4cpp/serialization/LittleEndian.h>

namespace ser4cpp
{
namespace serializers
{

    // To use LittleEndian::write(...)
    template<> inline bool write_one(wseq_t& dest, const opendnp3::DNPTime& value)
    {
        return UInt48::write_to(dest, UInt48Type(value.value));
    }

    template<> inline bool read_one(rseq_t& input, opendnp3::DNPTime& out)
    {
        UInt48Type temp;
        bool result = UInt48::read_from(input, temp);
        out.value = temp.Get();
        return result;
    }

} // namespace serializers
} // namespace ser4cpp

#endif
