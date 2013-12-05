#include <opendnp3/OctetData.h>

#include <assert.h>
#include <memory.h>

namespace opendnp3
{

OctetData::OctetData() :  mpData(nullptr), mSize(0)
{

}

OctetData::OctetData(const uint8_t* apValue, size_t aSize) : mpData(nullptr), mSize(0)	
{
	assert(aSize <= MAX_SIZE);
	mpData = new uint8_t[aSize];
	memcpy(mpData, apValue, aSize);
	mSize = aSize;
}

OctetData& OctetData::operator=( const OctetData& rhs )
{
	if(&rhs != this) {
		assert(rhs.mSize <= MAX_SIZE);
		if(mpData != nullptr)
		{
			delete[] mpData;
			mpData = nullptr;
			mSize = 0;
		}
		mpData = new uint8_t[rhs.mSize];
		memcpy(mpData, rhs.mpData, rhs.mSize);
		mSize = rhs.mSize;
	}
	return *this;
}


OctetData::OctetData(const OctetData& arCopy) : mpData(nullptr), mSize(0)
{
	mSize = arCopy.GetSize();
	mpData = new uint8_t[mSize];
	memcpy(mpData, arCopy.Data(), mSize);	
}

OctetData::~OctetData()
{
	if(mpData != nullptr) {
		delete[] mpData;
		mpData = nullptr;
	}
}

const uint8_t* OctetData::Data() const
{
	return mpData;
}

std::string OctetData::AsString() const
{
	return std::string(reinterpret_cast<char*>(mpData), mSize);
}

size_t OctetData::GetSize() const
{
	return this->mSize;
}


}

/* vim: set ts=4 sw=4: */


