//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __INDEXED_WRITE_ITERATOR_H_
#define __INDEXED_WRITE_ITERATOR_H_

#include <APL/Types.h>
#include "APDUConstants.h"
#include "ObjectHeader.h"
#include <assert.h>
#include <stddef.h>

namespace apl
{
namespace dnp
{

/**
Buffer iterator to write objects prefixed with specific indices.
*/
class IndexedWriteIterator
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
}

#endif

