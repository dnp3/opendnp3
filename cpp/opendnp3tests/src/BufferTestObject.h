#ifndef __BUFFER_TEST_OBJECT_H_
#define __BUFFER_TEST_OBJECT_H_

#include <string>
#include <vector>
#include <cstdint>

#include <openpal/BufferWrapper.h>

namespace opendnp3
{
class ByteStr;

class BufferTestObject
{
	static const size_t MAX_SIZE = 1024 * 1024;

public:
	BufferTestObject();
	~BufferTestObject();

	bool BufferEquals(const openpal::ReadOnlyBuffer& arBuffer) const;
	bool BufferEquals(const uint8_t*, size_t) const;
	bool BufferEqualsHex(const std::string& arData) const;
	bool BufferEqualsString(const std::string& arData) const;
	bool BufferContains(const std::string& arData) const;

	std::string GetBufferAsHexString(bool spaced = true) const;

	bool IsBufferEmpty() {
		return mBuffer.size() == 0;
	}
	void ClearBuffer();
	size_t Size() {
		return mBuffer.size();
	}
	bool SizeEquals(size_t aNum) {
		return aNum == Size();
	}
	size_t NumWrites() {
		return mNumWrites;
	}

protected:
	void WriteToBuffer(const openpal::ReadOnlyBuffer& arBuffer);

private:
	size_t mNumWrites;
	std::vector<uint8_t> mBuffer;
};

}

#endif

