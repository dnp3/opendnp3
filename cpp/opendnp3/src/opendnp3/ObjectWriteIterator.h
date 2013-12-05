#ifndef __OBJECT_WRITE_ITERATOR_H_
#define __OBJECT_WRITE_ITERATOR_H_

#include <openpal/Exception.h>
#include <openpal/Location.h>
#include <opendnp3/APDUConstants.h>
#include <openpal/Location.h>

#include "ObjectHeader.h"

#include <assert.h>
#include <stddef.h>

namespace opendnp3
{

/**
Buffer iterator to write objects with continuous indices.
*/
class DLL_LOCAL ObjectWriteIterator
{

	friend class APDU;

public:

	const ObjectWriteIterator& operator++();

	bool IsEnd() const {
		return mIndex > mStop;
	};

	uint8_t* operator*() const;

private:

	ObjectWriteIterator();

	ObjectWriteIterator(uint8_t* apPos, size_t aStart, size_t aStop, size_t aObjectSize);

	uint8_t* mpPos;

	size_t mIndex;
	size_t mStart;
	size_t mStop;
	size_t mObjectSize;
};

inline uint8_t* ObjectWriteIterator::operator*() const
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "End of iteration");
	}
	return mpPos;
}

}

#endif

