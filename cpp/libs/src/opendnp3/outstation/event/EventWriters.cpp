/*
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

#include "EventWriters.h"

namespace opendnp3
{

class OctetStringSerializier : public DNP3Serializer<OctetString> 
{	
public:
	OctetStringSerializier(uint8_t size) : DNP3Serializer(GroupVariationID(110, size), size, nullptr, nullptr)
	{}
};

class OctetStringEventWriter : public IEventWriter<OctetString>
{	
	struct Serializer : public DNP3Serializer<OctetString>
	{	
		Serializer(uint8_t size) : DNP3Serializer(
			GroupVariationID(111, size), 
			size,
			nullptr, // won't be used for reading
			&Serializer::Write
		)
		{}

		static bool Write(const OctetString& value, openpal::WSlice& buffer)
		{			
			if (value.Size() > buffer.Size()) return false;
			value.ToRSlice().CopyTo(buffer);
			return true;
		}
	};

	const Serializer serializer;
	PrefixedWriteIterator<openpal::UInt16, OctetString> iterator;

public:

	OctetStringEventWriter(HeaderWriter& writer, uint8_t size) : 		
		serializer(size),
		iterator(writer.IterateOverCountWithPrefix<openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, serializer))
	{}

	virtual bool Write(const OctetString& meas, uint16_t index) override
	{
		if (meas.Size() != this->serializer.Size()) return false;
		
		return iterator.Write(meas, index);
	}
};

uint16_t EventWriters::Write(uint8_t firstSize, HeaderWriter& writer, IEventCollection<OctetString>& items)
{
	OctetStringEventWriter handler(writer, firstSize);
	return items.WriteSome(handler);
}

}


