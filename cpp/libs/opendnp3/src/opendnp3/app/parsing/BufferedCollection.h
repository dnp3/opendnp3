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
#ifndef OPENDNP3_BUFFEREDCOLLECTION_H
#define OPENDNP3_BUFFEREDCOLLECTION_H

#include "ICollection.h"

namespace opendnp3
{

template <class T, class ReadFunc>
class BufferedCollection : public ICollection<T>
{
public:

	BufferedCollection(const openpal::ReadBufferView& buffer_, uint32_t count, const ReadFunc& readFunc_) :	
		buffer(buffer_),
		COUNT(count),
		readFunc(readFunc_)
	{}

	virtual uint32_t Count() const override final { return COUNT; }


	virtual void Foreach(IVisitor<T>& visitor) const final
	{
		openpal::ReadBufferView copy(buffer);

		for (uint32_t pos = 0; pos < COUNT; ++pos)
		{
			visitor.OnValue(readFunc(copy, pos));
		}		
	}

private:

	openpal::ReadBufferView buffer;
	const uint32_t COUNT;
	ReadFunc readFunc;
};

template <class T, class ReadFunc>
BufferedCollection<T, ReadFunc> CreateBufferedCollection(const openpal::ReadBufferView& buffer, uint32_t count, const ReadFunc& readFunc)
{
	return BufferedCollection<T, ReadFunc>(buffer, count, readFunc);
}

}

#endif
