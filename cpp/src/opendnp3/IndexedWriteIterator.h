
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

#ifndef __INDEXED_WRITE_ITERATOR_H_
#define __INDEXED_WRITE_ITERATOR_H_

#include <opendnp3/Types.h>
#include <opendnp3/APDUConstants.h>
#include <opendnp3/Visibility.h>

#include "ObjectHeader.h"

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

