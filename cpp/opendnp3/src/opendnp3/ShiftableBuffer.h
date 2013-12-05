#ifndef __SHIFTABLE_BUFFER_H_
#define __SHIFTABLE_BUFFER_H_

#include <openpal/Visibility.h>

#include <stddef.h>
#include <cstdint>

namespace opendnp3
{


/** @section DESCRIPTION
		Implements a buffer that can shift its contents as it is read */
class DLL_LOCAL ShiftableBuffer
{
public:
	/** @param aSize Size of the buffer in bytes */
	ShiftableBuffer( size_t aSize);

	/**
	 * Construct a buffer with the specified content and size.
	 *
	 * @param aBuffer    The content to initialize this ShiftableBuffer to.
	 * @param aSize      The size of aBuffer and the max size of this ShiftableBuffer.
	 */
	ShiftableBuffer( const uint8_t* aBuffer, size_t aSize);
	~ShiftableBuffer();

	////////////////////////////////////////////
	// Functions related to reading
	////////////////////////////////////////////

	/** @return Bytes available to be read */
	size_t NumReadBytes() const;

	/** @return the value of the ith byte in read subsequence of the buffer */
	const uint8_t& operator[](size_t index) const;

	/** @return Pointer to the next byte to be read in the buffer */
	const uint8_t* ReadBuff() const;

	/** Signal that some bytes don't have to be stored any longer. They'll be recovered during the next shift operation. */
	void AdvanceRead(size_t aNumBytes);

	////////////////////////////////////////////
	// Functions related to writing
	////////////////////////////////////////////

	/** Shift the buffer back to front, writing over bytes that have already been read. The objective
		being to free space for further writing. */
	void Shift();

	/** Reset the buffer to its initial state, empty */
	void Reset();

	/** @return Bytes of available for writing */
	size_t NumWriteBytes() const;
	/** @return Pointer to the position in the buffer available for writing */
	uint8_t* WriteBuff() const;
	/** Signal to the buffer bytes were written to the current write position */
	void AdvanceWrite(size_t aNumBytes);

	////////////////////////////////////////////
	// Other functions
	////////////////////////////////////////////

	/**
		Searches the read subsequence for a pattern. If a match is found, the reader is advanced to the beginning of the match.
		If a partial match is found at the end of the read subsequence, the partial match is perserved.

		i.e. if the pattern is {CBC} and the unread buffer is {AACB}, the unread buffer after the sync
		will be {CB}.

		If no match is found, the reader position is advanced to the end of the read subsequence.

		@param apPattern - Pointer to first byte in the input pattern buffer
		@param aNumBytes - Number of bytes in the pattern
		@return true if the full pattern was found in the buffer

	*/
	bool Sync(const uint8_t* apPattern, size_t aNumBytes);

private:

	// Recursive function called by Sync
	size_t SyncSubsequence(const uint8_t* apPattern, size_t aNumPatternBytes, size_t aOffset);



	uint8_t* mpBuffer;
	const size_t M_SIZE;
	size_t mWritePos;
	size_t mReadPos;
};

inline const uint8_t& ShiftableBuffer::operator[](size_t i) const
{
	return ReadBuff()[i];
}
inline const uint8_t* ShiftableBuffer::ReadBuff() const
{
	return mpBuffer + mReadPos;
}
inline size_t ShiftableBuffer::NumReadBytes() const
{
	return mWritePos - mReadPos;
}

inline size_t ShiftableBuffer::NumWriteBytes() const
{
	return M_SIZE - mWritePos;
}
inline uint8_t* ShiftableBuffer::WriteBuff() const
{
	return mpBuffer + mWritePos;
}


}

#endif
