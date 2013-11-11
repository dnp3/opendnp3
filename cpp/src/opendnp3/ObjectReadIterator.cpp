
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "ObjectReadIterator.h"
#include "ObjectHeader.h"
#include "Objects.h"

#include <openpal/Exception.h>

using namespace openpal;

namespace opendnp3
{

ObjectReadIterator::ObjectReadIterator(const HeaderInfo& arInfo, const uint8_t* apBuffer, bool aHasData) :
	mHeaderInfo(arInfo),
	mCurrentObjectNum(0),
	mpPrefixPos(NULL),
	mpBuffer(apBuffer),
	mHasData(aHasData)
{
	if(!this->IsEnd()) {
		mpPrefixPos = mpBuffer + (mHeaderInfo.GetPosition() + mHeaderInfo.GetHeaderSize());
		this->SetObjectInfo();
	}

	if(mHeaderInfo.GetObjectType() == OT_BITFIELD) {
		mInfo.mStart = mInfo.mIndex;
	}
	else {
		mInfo.mStart = 0; //not used in conjunction with other objects
	}
}

void ObjectReadIterator::SetObjectInfo()
{
	if(mHasData) mpPosition = mpPrefixPos + this->mHeaderInfo.GetPrefixSize();
	mInfo.mIndex = this->CalcIndex();
}

size_t ObjectReadIterator::CalcObjSize(const uint8_t* apPrefixPos)
{
	if(!mHasData) return 0;

	switch(mHeaderInfo.GetQualifier()) {

	case(QC_1B_VCNT_1B_SIZE):
		return UInt8::Read(apPrefixPos);
	case(QC_1B_VCNT_2B_SIZE):
		return UInt16LE::Read(apPrefixPos);
	case(QC_1B_VCNT_4B_SIZE):
		return UInt32LE::Read(apPrefixPos);
	default:
		//for all other types, the size should be static or collective
		switch(mHeaderInfo.GetObjectType()) {
		case(OT_FIXED):
			return static_cast<const FixedObject*>(mHeaderInfo.GetBaseObject())->GetSize();
		case(OT_BITFIELD):
			return static_cast<const BitfieldObject*>(mHeaderInfo.GetBaseObject())->GetSize(mHeaderInfo.GetCount());
		default:
			break;
		}
		MACRO_THROW_EXCEPTION(Exception, "Invalid object type for non-sized prefix");
	}
}

// calculate the index of the current object, assumes that the
// stream pointer is already set to the beginning of the prefix (or object if no prefix)
// Some objects don't have an index associated with them so index just gets set
// to there relative position in the header
size_t ObjectReadIterator::CalcIndex()
{
	size_t index = 0;

	switch(mHeaderInfo.GetHeaderType()) {
		//For a count you have to examine the prefix code
	case(OHT_COUNT_1_OCTET):
	case(OHT_COUNT_2_OCTET):
	case(OHT_COUNT_4_OCTET):
		index = this->CalcCountIndex(mHeaderInfo.GetQualifier(), mpPrefixPos);
		break;
		// these must be used with no-prefix so
		// you can just examine the range and current object number
	case(OHT_RANGED_2_OCTET):
	case(OHT_RANGED_4_OCTET):
	case(OHT_RANGED_8_OCTET): {
			RangeInfo range;
			static_cast<const IRangeHeader*>(mHeaderInfo.GetHeader())->GetRange(mpBuffer + mHeaderInfo.GetPosition(), range);
			index = range.Start + mCurrentObjectNum;
			break;
		}
	default:
		break; // All objects should just use an index of zero since it shouldn't contain objects
	}

	return index;
}

size_t ObjectReadIterator::CalcCountIndex(QualifierCode aCode, const uint8_t* apPrefixPos)
{
	switch(aCode) {

		// if there's no prefix or it's prefixed
		// with a size assume the index is implicit
		// based on the object number in the sequence from 0
	case(QC_1B_CNT):
	case(QC_2B_CNT):
	case(QC_4B_CNT):
	case(QC_ALL_OBJ):
		return this->mCurrentObjectNum;
		// if it's prefixed with an index, return the index
	case(QC_1B_CNT_1B_INDEX):
		return UInt8::Read(apPrefixPos);
	case(QC_2B_CNT_2B_INDEX):
		return UInt16LE::Read(apPrefixPos);
	case(QC_4B_CNT_4B_INDEX):
		return UInt32LE::Read(apPrefixPos);
	default:
		MACRO_THROW_EXCEPTION(Exception, "Invalid qualifier code");
	}
}

}

