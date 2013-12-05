#include "ShiftableBuffer.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>

#include <memory.h>

using namespace openpal;

namespace opendnp3
{

ShiftableBuffer::ShiftableBuffer(size_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
}


ShiftableBuffer::ShiftableBuffer( const uint8_t* aBuffer, size_t aSize) :
	mpBuffer(new uint8_t[aSize]),
	M_SIZE(aSize),
	mWritePos(0),
	mReadPos(0)
{
	if( aBuffer) {
		memcpy( mpBuffer, aBuffer, aSize) ;
		mWritePos += aSize ;
	}
}


void ShiftableBuffer::Shift()
{
	//copy all unread data to the front of the buffer
	memmove(mpBuffer, this->ReadBuff(), this->NumReadBytes());
	mWritePos = this->NumReadBytes();
	mReadPos = 0;
}

void ShiftableBuffer::Reset()
{
	mWritePos = 0;
	mReadPos = 0;
}

ShiftableBuffer::~ShiftableBuffer()
{
	delete[] mpBuffer;
}


void ShiftableBuffer::AdvanceRead(size_t aNumBytes)
{
	if(aNumBytes > this->NumReadBytes()) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Cannot be greater than the number of currently available reader bytes");
	}

	mReadPos += aNumBytes;
}

void ShiftableBuffer::AdvanceWrite(size_t aNumBytes)
{

	if(aNumBytes > this->NumWriteBytes()) // This could indicate a buffer overflow
		MACRO_THROW_EXCEPTION(ArgumentException, "Cannot be greater than the number of currently available writer bytes");

	mWritePos += aNumBytes;
}

bool ShiftableBuffer::Sync(const uint8_t* apPattern, size_t aNumBytes)
{
	if(aNumBytes < 1) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Pattern must be at least 1 byte");
	}

	size_t offset = SyncSubsequence(apPattern, aNumBytes, 0);
	bool res = (this->NumReadBytes() - offset) >= aNumBytes;
	if(offset > 0) this->AdvanceRead(offset);

	return res;
}

size_t ShiftableBuffer::SyncSubsequence(const uint8_t* apPattern, size_t aNumPatternBytes, size_t aOffset)
{
	size_t read_bytes = this->NumReadBytes() - aOffset;
	if(aNumPatternBytes > read_bytes) aNumPatternBytes = read_bytes;

	const uint8_t* pRead = this->ReadBuff() + aOffset;

	for(size_t i = 0; i < aNumPatternBytes; ++i) {
		if(apPattern[i] != pRead[i])
			return SyncSubsequence(apPattern, aNumPatternBytes, aOffset + 1);
	}

	return aOffset;
}

}
