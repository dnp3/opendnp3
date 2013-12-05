#ifndef __COPYABLE_BUFFER_H_
#define __COPYABLE_BUFFER_H_

#include <stddef.h>
#include <memory>
#include <sstream>

#include <openpal/BufferWrapper.h>

namespace opendnp3
{

/** Implements a dynamic buffer with a safe
	copy constructor. This makes it easier to compose with
	classes without requiring an explicit copy constructor
*/
class CopyableBuffer
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	friend std::ostream& operator<<(std::ostream& output, const CopyableBuffer& arBuff);
#endif

public:
	// Construct null buffer
	CopyableBuffer();
	// Construct based on starting size of buffer
	CopyableBuffer(size_t aSize);
	CopyableBuffer(const openpal::ReadOnlyBuffer&);
	CopyableBuffer(const uint8_t* apBuff, size_t aSize);
	CopyableBuffer(const CopyableBuffer&);
	CopyableBuffer& operator=(const CopyableBuffer&);
	~CopyableBuffer();

	bool operator==( const CopyableBuffer& other) const;
	bool operator!=( const CopyableBuffer& other) const {
		return ! (*this == other);
	}

	openpal::ReadOnlyBuffer ToReadOnly() const
	{
		return openpal::ReadOnlyBuffer(mpBuff, mSize);
	}

	operator const uint8_t* () const {
		return mpBuff;
	}
	
	operator uint8_t* () {
		return mpBuff;
	}

	size_t Size() const {
		return mSize;
	}

	void Zero();

protected:
	uint8_t* mpBuff;

private:
	size_t mSize;
};


}

#endif


