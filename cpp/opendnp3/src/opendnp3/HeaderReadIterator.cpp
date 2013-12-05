#include "HeaderReadIterator.h"
#include "ObjectReadIterator.h"

namespace opendnp3
{

HeaderReadIterator::HeaderReadIterator(const std::vector<HeaderInfo>* apHeaders, const uint8_t* apBuffer, bool aHasData) :
	mpHeaders(apHeaders),
	mpBuffer(apBuffer),
	mHasData(aHasData),
	mIndex(0)
{

}


ObjectReadIterator HeaderReadIterator::BeginRead()
{
	return ObjectReadIterator((*mpHeaders)[mIndex], mpBuffer, mHasData);
}

}

