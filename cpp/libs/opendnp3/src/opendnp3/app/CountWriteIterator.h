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
#ifndef __COUNT_WRITE_ITERATOR_H_
#define __COUNT_WRITE_ITERATOR_H_


#include <openpal/WriteBuffer.h>
#include <openpal/ISerializer.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template <class CountType, class WriteType>
class CountWriteIterator
{
public:

	static CountWriteIterator Null()
	{
		auto buffer = openpal::WriteBuffer::Empty();
		return CountWriteIterator(nullptr, buffer); // TODO - this should be a pointer
	}

	CountWriteIterator(openpal::ISerializer<WriteType>* pSerializer_, openpal::WriteBuffer& aPosition) :
		count(0),
		pSerializer(pSerializer_),
		countPosition(aPosition),
		position(aPosition),
		isNull(aPosition.Size() < CountType::Size || pSerializer == nullptr)
	{
		if(!isNull)
		{
			position.Advance(CountType::Size);
		}
	}

	bool Complete()
	{
		if(isNull) return false;
		else
		{
			CountType::Write(countPosition, count);
			return true;
		}
	}

	bool Write(const WriteType& value)
	{
		if(isNull || position.Size() < pSerializer->Size()) return false;
		else
		{
			pSerializer->Write(value, position);
			++count;
			return true;
		}
	}

	bool IsNull() const
	{
		return isNull;
	}

private:

	typename CountType::Type count;
	openpal::ISerializer<WriteType>* pSerializer;

	bool isNull;

	openpal::WriteBuffer countPosition;  // make a copy to record where we write the count
	openpal::WriteBuffer& position;
};

}

#endif
