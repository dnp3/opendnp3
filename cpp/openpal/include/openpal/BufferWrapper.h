#ifndef __BUFFER_WRAPPER_H_
#define __BUFFER_WRAPPER_H_

#include <cstddef>
#include <cstdint>

namespace openpal
{

class HasSize
{

public:
	HasSize(size_t aSize);

	size_t Size() const;

	bool IsEmpty() const;

	bool IsNotEmpty() const;

protected:

	size_t mSize;

};

	
class ReadOnlyBuffer : public HasSize
{

public:

	ReadOnlyBuffer();
	ReadOnlyBuffer(uint8_t const* apBuffer, size_t aSize);

	void CopyTo(uint8_t* apDest) const;

	void Advance(size_t aNum);

	operator uint8_t const *() const { return mpBuffer; };

private:
	uint8_t const* mpBuffer;
	
};

class WriteBuffer : public HasSize
{
	public:

    WriteBuffer();
	WriteBuffer(uint8_t* apBuffer, size_t aSize);

	void Clear();

	void Advance(size_t aNum);

	ReadOnlyBuffer ToReadOnly() const;

	operator uint8_t *() { return mpBuffer; };

	private:

	uint8_t* mpBuffer;
};


}

#endif
