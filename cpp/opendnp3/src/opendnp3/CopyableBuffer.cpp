#include "CopyableBuffer.h"

#include <memory.h>

#include <openpal/ToHex.h>

namespace opendnp3
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::ostream& operator<<(std::ostream& output, const CopyableBuffer& arBuff)
{
	output << "[" << openpal::toHex(arBuff.ToReadOnly(), true) << "]";
	return output;
}
#endif

CopyableBuffer::CopyableBuffer() :
	mpBuff(nullptr),
	mSize(0)
{

}

CopyableBuffer::CopyableBuffer(size_t aSize) :
	mpBuff(new uint8_t[aSize]),
	mSize(aSize)
{
	this->Zero();
}

CopyableBuffer::CopyableBuffer(const openpal::ReadOnlyBuffer& arBuffer) :
	mpBuff(new uint8_t[arBuffer.Size()]),
	mSize(arBuffer.Size())
{
	arBuffer.CopyTo(mpBuff);	
}

CopyableBuffer::CopyableBuffer(const uint8_t* apBuff, size_t aSize) :
	mpBuff(new uint8_t[aSize]),
	mSize(aSize)
{
	memcpy(mpBuff, apBuff, aSize);
}

CopyableBuffer::CopyableBuffer(const CopyableBuffer& arBuffer) :
	mpBuff(new uint8_t[arBuffer.Size()]),
	mSize(arBuffer.Size())
{
	memcpy(mpBuff, arBuffer, mSize);
}

void CopyableBuffer::Zero()
{
	memset(mpBuff, 0, mSize);
}

CopyableBuffer& CopyableBuffer::operator=(const CopyableBuffer& arRHS)
{
	//check for assignment to self
	if(this == &arRHS) return *this;

	if(arRHS.Size() != mSize) {
		mSize = arRHS.Size();
		delete mpBuff;
		mpBuff = new uint8_t[mSize];
	}

	memcpy(mpBuff, arRHS, mSize);

	return *this;
}

CopyableBuffer::~CopyableBuffer()
{
	delete [] mpBuff;
}

bool CopyableBuffer::operator==( const CopyableBuffer& other) const
{
	if(other.Size() != this->Size()) return false;
	else {
		for(size_t i = 0; i < this->Size(); ++i) {
			if(this->mpBuff[i] != other.mpBuff[i]) return false;
		}

		return true;
	}
}

}
