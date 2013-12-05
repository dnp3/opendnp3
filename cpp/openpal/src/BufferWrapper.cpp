#include <openpal/BufferWrapper.h>

#include <cstring>
#include <assert.h>

namespace openpal
{
	HasSize::HasSize(size_t aSize) : mSize(aSize)
	{}

	size_t HasSize::Size() const
	{
		return mSize;
	}

	bool HasSize::IsNotEmpty() const
	{
		return mSize > 0;
	}

	bool HasSize::IsEmpty() const
	{
		return mSize == 0;
	}

	ReadOnlyBuffer::ReadOnlyBuffer(): HasSize(0), mpBuffer(nullptr)
	{
	
	}

	ReadOnlyBuffer::ReadOnlyBuffer(uint8_t const* apBuffer, size_t aSize) :
		HasSize(aSize),
		mpBuffer(apBuffer)
	{}

	void ReadOnlyBuffer::CopyTo(uint8_t* apDest) const
	{
		memcpy(apDest, mpBuffer, mSize);
	}

	void ReadOnlyBuffer::Advance(size_t aNum)
	{
		assert(aNum <= mSize);
		mpBuffer += aNum;
		mSize -= aNum;
	}

	WriteBuffer::WriteBuffer(): HasSize(0), mpBuffer(nullptr)
	{
		
	}

	WriteBuffer::WriteBuffer(uint8_t* apBuffer, size_t aSize) :
		HasSize(aSize),
		mpBuffer(apBuffer)
	{}

	void WriteBuffer::Clear()
	{
		mpBuffer = nullptr;
		mSize = 0;
	}

	void WriteBuffer::Advance(size_t aNum)
	{
		assert(aNum <= mSize);
		mpBuffer += aNum;
		mSize -= aNum;
	}

	ReadOnlyBuffer WriteBuffer::ToReadOnly() const
	{
		return ReadOnlyBuffer(mpBuffer, mSize);
	}

}


