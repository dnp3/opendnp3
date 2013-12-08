/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
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
class ObjectWriteIterator
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

