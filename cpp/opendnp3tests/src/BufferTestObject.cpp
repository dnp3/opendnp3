#include "BufferTestObject.h"

#include "BufferHelpers.h"

#include <memory>

#include <openpal/Exception.h>
#include <openpal/Location.h>
#include <openpal/ToHex.h>


using namespace openpal;

namespace opendnp3
{

BufferTestObject::BufferTestObject() :
	mNumWrites(0)
{

}

BufferTestObject::~BufferTestObject()
{

}

void BufferTestObject::ClearBuffer()
{
	mNumWrites = 0;
	mBuffer.clear();
}

bool BufferTestObject::BufferEquals(const openpal::ReadOnlyBuffer& arBuffer) const
{
	return BufferEquals(arBuffer, arBuffer.Size());
}

bool BufferTestObject::BufferEquals(const uint8_t* apData, size_t aNumBytes) const
{

	if(aNumBytes != mBuffer.size()) return false;
	for(size_t i = 0; i < aNumBytes; i++)
		if(apData[i] != mBuffer[i]) {
			return false;
		}
	return true;
}

bool BufferTestObject::BufferContains(const std::string& arPattern) const
{
	std::string s;
	for(size_t i = 0; i < mBuffer.size(); ++i) {
		std::string c(1, static_cast<char>(mBuffer[i]));
		s.append(c);
	}
	return s.find(arPattern) != std::string::npos;
}

std::string BufferTestObject::GetBufferAsHexString(bool spaced) const
{	
	CopyableBuffer buffer(mBuffer.size());
	for(size_t i=0; i<mBuffer.size(); ++i) buffer[i] = mBuffer[i];
	return toHex(buffer, buffer.Size(), spaced);
}


bool BufferTestObject::BufferEqualsHex(const std::string& arData) const
{
	HexSequence hs(arData);
	return BufferEquals(hs, hs.Size());
}

bool BufferTestObject::BufferEqualsString(const std::string& arData) const
{
	if(arData.size() != mBuffer.size()) return false;
	for(size_t i = 0; i < mBuffer.size(); i++)
		if(arData[i] != mBuffer[i]) {
			return false;
		}
	return true;
}

void BufferTestObject::WriteToBuffer(const ReadOnlyBuffer& arBuffer)
{
	if( (mBuffer.size() + arBuffer.Size()) > MAX_SIZE ) throw Exception(LOCATION, "Max size exceeded");
	++mNumWrites;
	for(size_t i = 0; i < arBuffer.Size(); ++i) mBuffer.push_back(arBuffer[i]);
}

} //end namespace

