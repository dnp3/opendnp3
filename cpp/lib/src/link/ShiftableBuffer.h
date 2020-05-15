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
#ifndef OPENDNP3_SHIFTABLEBUFFER_H
#define OPENDNP3_SHIFTABLEBUFFER_H

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

/** @section DESCRIPTION
        Implements a buffer that can shift its contents as it is read */
class ShiftableBuffer
{
public:
    /**
     * Construct the facade over the specified underlying buffer
     */
    ShiftableBuffer(uint8_t* pBuffer_, size_t size);

    // ------- Functions related to reading -----------

    size_t NumBytesRead() const
    {
        return writePos - readPos;
    }

    /// @return Pointer to the next byte to be read in the buffer
    ser4cpp::rseq_t ReadBuffer() const
    {
        return ser4cpp::rseq_t(pBuffer + readPos, NumBytesRead());
    }

    /// Signal that some bytes don't have to be stored any longer. They'll be recovered during the next shift operation.
    void AdvanceRead(size_t aNumBytes);

    // ------- Functions related to writing -----------

    /// Shift the buffer back to front, writing over bytes that have already been read. The objective
    /// being to free space for further writing.
    void Shift();

    /// Reset the buffer to its initial state, empty
    void Reset();

    /// @return Bytes of available for writing
    size_t NumWriteBytes() const
    {
        return M_SIZE - writePos;
    }

    /// @return Pointer to the position in the buffer available for writing
    uint8_t* WriteBuff() const
    {
        return pBuffer + writePos;
    }

    /// Signal to the buffer bytes were written to the current write position
    void AdvanceWrite(size_t numBytes);

    ////////////////////////////////////////////
    // Other functions
    ////////////////////////////////////////////

    /// Searches the read subsequence for 0x0564 sync bytes
    /// @return true if both sync bytes were found in the buffer.
    bool Sync(size_t& skipCount);

private:
    uint8_t* pBuffer;
    const size_t M_SIZE;
    size_t writePos;
    size_t readPos;
};

} // namespace opendnp3

#endif
