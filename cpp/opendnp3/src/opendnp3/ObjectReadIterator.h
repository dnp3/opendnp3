#ifndef __OBJECT_READ_ITERATOR_H_
#define __OBJECT_READ_ITERATOR_H_

#include <assert.h>

#include <opendnp3/ObjectInterfaces.h>
#include <openpal/Visibility.h>

#include "HeaderReadIterator.h"
#include "ObjectHeader.h"

namespace opendnp3
{

class DLL_LOCAL ObjectInfo
{
	friend class ObjectReadIterator;

	ObjectInfo(size_t aIndex, size_t aStart) : mIndex(aIndex), mStart(aStart)
	{}

	ObjectInfo() : mIndex(0), mStart(0)
	{}

public:

	size_t Index() const {
		return mIndex;
	}
	size_t Start() const {
		return mStart;
	}

private:

	size_t mIndex;		// the objects device-based index
	size_t mStart;		// used in conjunction with bitfield objects only,
};

/**
 * An iterator that clients can use to loop over the object data in an
 * APDU object.  This class is usually used in conjunction with the
 * HeaderReadIterator class.
*/
class DLL_LOCAL ObjectReadIterator
{
	friend class HeaderReadIterator;

public:

	bool HasData() {
		return mHasData;
	}
	const ObjectInfo* operator->() const;
	const uint8_t* operator*() const;

	const ObjectReadIterator& operator++();   //prefix version
	const ObjectReadIterator operator++(int); //postfix version
	const HeaderInfo& Header() {
		return mHeaderInfo;
	}

	size_t Count() const {
		return mHeaderInfo.GetCount();
	}
	bool IsEnd() const {
		return mCurrentObjectNum >= Count();
	}

private:

	ObjectReadIterator(const HeaderInfo& arInfo, const uint8_t* apBuffer, bool aHasData);
	HeaderInfo mHeaderInfo;
	size_t mCurrentObjectNum;
	ObjectInfo mInfo;
	const uint8_t* mpPrefixPos;
	const uint8_t* mpPosition;
	const uint8_t* mpBuffer;
	bool mHasData;

	void SetObjectInfo();

	//private helpers
	size_t CalcIndex();
	size_t CalcCountIndex(QualifierCode aCode, const uint8_t* apPrefixPos);
	size_t CalcObjSize(const uint8_t* apPrefixPos);
};

inline const ObjectInfo* ObjectReadIterator::operator->() const
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_ITERATOR_OUT_OF_BOUNDS);
	}
	return &mInfo;
}

inline const uint8_t* ObjectReadIterator::operator*() const
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_ITERATOR_OUT_OF_BOUNDS);
	}
	if(!mHasData) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_ITERATOR_NO_DATA);
	}
	return mpPosition;
}

inline const ObjectReadIterator& ObjectReadIterator::operator++()
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_ITERATOR_OUT_OF_BOUNDS);
	}

	++mCurrentObjectNum;

	if(!this->IsEnd()) {
		//Don't advance the prefix position if the object is a bitfield
		if(mHeaderInfo.GetObjectType() != OT_BITFIELD) {
			mpPrefixPos += mHeaderInfo.GetPrefixSize() + this->CalcObjSize(mpPrefixPos);
		}

		this->SetObjectInfo();
	}

	return (*this);
}

inline const ObjectReadIterator ObjectReadIterator::operator++(int)
{
	ObjectReadIterator tmp(*this);
	++(*this);
	return tmp;
}

}

/* vim: set ts=4 sw=4: */

#endif

