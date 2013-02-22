
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "IndexedWriteIterator.h"


#include "ObjectHeader.h"
#include "PackingUnpacking.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Location.h>

namespace opendnp3
{

IndexedWriteIterator::IndexedWriteIterator() :
	mpPos(NULL),
	mIndexMode(IM_NONE),
	mIndex(0),
	mCount(0),
	mObjectSize(0),
	mIndexSet(false)
{

}

IndexedWriteIterator::IndexedWriteIterator(uint8_t* apPos, size_t aCount, QualifierCode aCode, size_t aObjectSize) :
	mpPos(apPos),
	mIndexMode(GetIndexMode(aCode)),
	mIndex(0),
	mCount(aCount),
	mObjectSize(aObjectSize),
	mIndexSet(false)
{

}

uint8_t* IndexedWriteIterator::operator*() const
{
	if(!mIndexSet) MACRO_THROW_EXCEPTION(InvalidStateException, "Index has not been written");
	return mpPos + mIndexMode;
}

IndexedWriteIterator::IndexMode IndexedWriteIterator::GetIndexMode(QualifierCode aCode)
{
	switch(aCode) {
	case(QC_1B_CNT_1B_INDEX): return IM_1B;
	case(QC_2B_CNT_2B_INDEX): return IM_2B;
	case(QC_4B_CNT_4B_INDEX): return IM_4B;

	case(QC_1B_CNT):
	case(QC_2B_CNT):
	case(QC_4B_CNT):
		return IM_NONE;

	default:
		MACRO_THROW_EXCEPTION(Exception, "Illegal qualifer for packed indexed");
	}
}

void IndexedWriteIterator::SetIndex(size_t aIndex)
{
	if(mIndexSet) MACRO_THROW_EXCEPTION(InvalidStateException, "Index already set");
	if(IsEnd()) MACRO_THROW_EXCEPTION(InvalidStateException, "End of iteration");

	switch(mIndexMode) {
	case(IM_1B):
		if(aIndex > UInt8::Max) MACRO_THROW_EXCEPTION(ArgumentException ,"");
		UInt8::Write(mpPos, static_cast<uint8_t>(aIndex));
		break;
	case(IM_2B):
		if(aIndex > UInt16LE::Max) MACRO_THROW_EXCEPTION(ArgumentException, "");
		UInt16LE::Write(mpPos, static_cast<uint16_t>(aIndex));
		break;
	case(IM_4B):
		if(aIndex > UInt32LE::Max) MACRO_THROW_EXCEPTION(ArgumentException, "");
		UInt32LE::Write(mpPos, static_cast<uint32_t>(aIndex));
		break;
	default:
		break;
	}

	mIndexSet = true;
}

const IndexedWriteIterator& IndexedWriteIterator::operator++()
{
	if(this->IsEnd()) MACRO_THROW_EXCEPTION(InvalidStateException, "End of iterattion");
	if(!mIndexSet) MACRO_THROW_EXCEPTION(InvalidStateException, "Index has not been set");

	++mIndex;
	mIndexSet = false;
	mpPos += mObjectSize + mIndexMode;

	return (*this);
}

const IndexedWriteIterator IndexedWriteIterator::operator++(int)
{
	IndexedWriteIterator tmp(*this);
	++(*this);
	return tmp;
}

}

