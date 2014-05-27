/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __SHIFTABLE_BUFFER_H_
#define __SHIFTABLE_BUFFER_H_



#include <stddef.h>
#include <cstdint>

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
	ShiftableBuffer(uint8_t* pBuffer_, uint32_t size);

	////////////////////////////////////////////
	// Functions related to reading
	////////////////////////////////////////////

	/** @return Bytes available to be read */
	uint32_t NumReadBytes() const;

	/** @return the value of the ith byte in read subsequence of the buffer */
	const uint8_t& operator[](uint32_t index) const;

	/** @return Pointer to the next byte to be read in the buffer */
	const uint8_t* ReadBuff() const;

	/** Signal that some bytes don't have to be stored any longer. They'll be recovered during the next shift operation. */
	void AdvanceRead(uint32_t aNumBytes);

	////////////////////////////////////////////
	// Functions related to writing
	////////////////////////////////////////////

	/** Shift the buffer back to front, writing over bytes that have already been read. The objective
		being to free space for further writing. */
	void Shift();

	/** Reset the buffer to its initial state, empty */
	void Reset();

	/** @return Bytes of available for writing */
	uint32_t NumWriteBytes() const;
	/** @return Pointer to the position in the buffer available for writing */
	uint8_t* WriteBuff() const;
	/** Signal to the buffer bytes were written to the current write position */
	void AdvanceWrite(uint32_t aNumBytes);

	////////////////////////////////////////////
	// Other functions
	////////////////////////////////////////////

	/**
		Searches the read subsequence for 0x0564 sync bytes. If a match is found, the reader is advanced to the beginning of the match.
		If a partial match is found at the end of the read subsequence, the partial match is perserved.

		i.e. if the pattern is {CBC} and the unread buffer is {AACB}, the unread buffer after the sync
		will be {CB}.

		If no match is found, the reader position is advanced to the end of the read subsequence.

		@return true if both sync bytes were found in the buffer.
	*/
	bool Sync();

private:

	uint8_t* pBuffer;
	const uint32_t M_SIZE;
	uint32_t writePos;
	uint32_t readPos;
};

inline const uint8_t& ShiftableBuffer::operator[](uint32_t i) const
{
	return ReadBuff()[i];
}
inline const uint8_t* ShiftableBuffer::ReadBuff() const
{
	return pBuffer + readPos;
}
inline uint32_t ShiftableBuffer::NumReadBytes() const
{
	return writePos - readPos;
}

inline uint32_t ShiftableBuffer::NumWriteBytes() const
{
	return M_SIZE - writePos;
}
inline uint8_t* ShiftableBuffer::WriteBuff() const
{
	return pBuffer + writePos;
}


}

#endif
