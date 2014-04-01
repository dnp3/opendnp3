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
#ifndef __PREFIXED_WRITE_ITERATOR_H_
#define __PREFIXED_WRITE_ITERATOR_H_

#include <limits>
#include "opendnp3/app/IDNP3Serializer.h"

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template <class PrefixType, class WriteType>
class PrefixedWriteIterator
{
public:

	static PrefixedWriteIterator Null()
	{
		PrefixedWriteIterator ret;
		return ret;
	}

	PrefixedWriteIterator() :
		pSerializer(nullptr),
		sizeOfTypePlusIndex(0),
		count(0),
		pPosition(nullptr),
		isNull(true)
	{}

	PrefixedWriteIterator(IDNP3Serializer<WriteType>* pSerializer_, openpal::WriteBuffer& position) :
		pSerializer(pSerializer_),
		sizeOfTypePlusIndex(pSerializer_->Size() + PrefixType::Size),
		count(0),
		countPosition(position),
		pPosition(&position),
		isNull(position.Size() < PrefixType::Size)
	{
		if(!isNull)
		{
			pPosition->Advance(PrefixType::Size);
		}
	}

	bool Complete()
	{
		if(isNull)
		{
			return false;
		}
		else
		{
			PrefixType::Write(countPosition, count);
			return true;
		}
	}

	bool Write(WriteType& value, typename PrefixType::Type index)
	{
		if(isNull || (pPosition->Size() < sizeOfTypePlusIndex) )
		{
			return false;
		}
		else
		{
			PrefixType::WriteBuffer(*pPosition, index);
			pSerializer->Write(value, *pPosition);
			++count;
			return true;
		}
	}

	bool IsNull() const
	{
		return isNull;
	}

private:

	IDNP3Serializer<WriteType>* pSerializer;
	uint32_t sizeOfTypePlusIndex;

	typename PrefixType::Type count;

	bool isNull;

	openpal::WriteBuffer countPosition;  // make a copy to record where we write the count
	openpal::WriteBuffer* pPosition;
};

}

#endif
