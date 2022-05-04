/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_LINKHEADER_H
#define OPENDNP3_LINKHEADER_H

#include "LinkLayerConstants.h"

#include "opendnp3/gen/LinkFunction.h"

namespace opendnp3
{

// Class for dealing with all aspects of FT3 Frame headers
struct LinkHeader
{
    LinkHeader();

    LinkHeader(uint8_t len, uint16_t src, uint16_t dest, bool aFromMaster, bool fcvdfc, bool fcb, LinkFunction aCode);

    // Setter

    void Set(uint8_t len, uint16_t src, uint16_t dest, bool aFromMaster, bool fcvdfc, bool fcb, LinkFunction aCode);

    void ChangeFCB(bool aFCB);

    uint8_t GetControl() const
    {
        return ctrl;
    }

    // Getters
    uint8_t GetLength() const
    {
        return length;
    }
    uint16_t GetDest() const
    {
        return dest;
    }
    uint16_t GetSrc() const
    {
        return src;
    }
    bool IsFromMaster() const
    {
        return (ctrl & MASK_DIR) != 0;
    }
    bool IsPriToSec() const
    {
        return (ctrl & MASK_PRM) != 0;
    }
    bool IsFcbSet() const
    {
        return (ctrl & MASK_FCB) != 0;
    }
    bool IsFcvDfcSet() const
    {
        return (ctrl & MASK_FCV) != 0;
    }
    uint8_t GetFuncByte() const
    {
        return ctrl & MASK_FUNC;
    }
    LinkFunction GetFuncEnum() const
    {
        return LinkFunctionSpec::from_type(ctrl & MASK_FUNC_OR_PRM);
    }

    bool ValidLength()
    {
        return length > 4;
    }

    /** Reads the header, setting all the fields. Does NOT validate 0x0564 or CRC
    @param apBuff Buffer of at least 10 bytes */
    void Read(const uint8_t* apBuff);

    /** Writes header to buffer including, 0x0564 and CRC
    @param apBuff Buffer of at least 10 bytes */
    void Write(uint8_t* apBuff) const;

    static uint8_t ControlByte(bool isMaster, bool fcb, bool fcvdfc, LinkFunction func);

private:
    // Fields read directly from the header
    uint8_t length; // Length of field, range [5,255] valid
    uint16_t src;   // Where the frame originated
    uint16_t dest;  // Where the frame is going
    uint8_t ctrl;   // Control octet, individual fields accessed using accessors below
};

} // namespace opendnp3

#endif
