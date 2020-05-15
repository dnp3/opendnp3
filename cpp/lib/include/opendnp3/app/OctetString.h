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
#ifndef OPENDNP3_OCTETSTRING_H
#define OPENDNP3_OCTETSTRING_H

#include "opendnp3/app/OctetData.h"

namespace opendnp3
{

/**
 * Respresents group 110/111 objects
 */
class OctetString : public OctetData
{
public:
    /**
     * Construct with a default value of [0x00] (length == 1)
     */
    OctetString() : OctetData() {}

    /**
     * Construct from a c-style string
     *
     * strlen() is used internally to determine the length
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied
     *
     * The null terminator is NOT copied as part of buffer
     */
    OctetString(const char* input) : OctetData(input) {}

    /**
     * Construct from read-only buffer slice
     *
     * If the length is 0, the default value of [0x00] is assigned
     * If the length is > 255, only the first 255 bytes are copied
     */
    OctetString(const Buffer& buffer) : OctetData(buffer) {}
};

} // namespace opendnp3

#endif
