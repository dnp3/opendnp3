#include "ObjectWriteIterator.h"

#include "ObjectHeader.h"

#include <openpal/Exception.h>

namespace opendnp3
{

ObjectWriteIterator::ObjectWriteIterator() :
	mpPos(nullptr),
	mIndex(1),
	mStart(0),
	mStop(0),
	mObjectSize(0)
{}


ObjectWriteIterator::ObjectWriteIterator(uint8_t* apPos, size_t aStart, size_t aStop, size_t aObjectSize) :
	mpPos(apPos),
	mIndex(aStart),
	mStart(aStart),
	mStop(aStop),
	mObjectSize(aObjectSize)
{

}

const ObjectWriteIterator& ObjectWriteIterator::operator++()
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "End of iterattion");
	}

	++mIndex;
	mpPos += mObjectSize;

	return (*this);
}

}

