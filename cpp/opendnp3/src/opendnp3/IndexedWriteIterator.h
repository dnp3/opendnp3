#ifndef __INDEXED_WRITE_ITERATOR_H_
#define __INDEXED_WRITE_ITERATOR_H_

#include <opendnp3/APDUConstants.h>
#include <openpal/Visibility.h>

#include "ObjectHeader.h"
#include "gen/QualifierCode.h"

#include <assert.h>
#include <stddef.h>

namespace opendnp3
{

/**
Buffer iterator to write objects prefixed with specific indices.
*/
class DLL_LOCAL IndexedWriteIterator
{
	friend class APDU;

public:

	IndexedWriteIterator();

	const IndexedWriteIterator& operator++();
	const IndexedWriteIterator operator++(int);
	uint8_t* operator*() const;

	void SetIndex(size_t aIndex);
	bool IsEnd() {
		return mIndex >= mCount;
	}
	size_t Count() {
		return mCount;
	}

private:

	IndexedWriteIterator(uint8_t* apPos, size_t aCount, QualifierCode aCode, size_t aObjectSize);

	enum IndexMode {
		IM_NONE = 0,
		IM_1B = 1,
		IM_2B = 2,
		IM_4B = 4
	};

	static IndexMode GetIndexMode(QualifierCode aCode);
	static size_t GetPrefixSize(IndexMode);

	uint8_t* mpPos;
	IndexMode mIndexMode;
	size_t mIndex;
	size_t mCount;
	size_t mObjectSize;
	bool mIndexSet;
};

}

#endif

