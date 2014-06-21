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
#include "OctetData.h"

#include <openpal/container/WriteBuffer.h>

using namespace openpal;

namespace opendnp3
{

OctetData::OctetData() :  size(0)
{

}

OctetData::OctetData(const ReadOnlyBuffer& buffer)
{
	Initialize(buffer);
}

void OctetData::Initialize(const ReadOnlyBuffer& buffer)
{
	size = static_cast<uint8_t>((buffer.Size() > MAX_SIZE) ? MAX_SIZE : buffer.Size());
	WriteBuffer dest(pData, MAX_SIZE);
	buffer.Take(size).CopyTo(dest);
}

OctetData& OctetData::operator=( const OctetData& rhs )
{
	if(&rhs != this)
	{
		this->Initialize(rhs.ToReadOnly());
	}
	return *this;
}

OctetData::OctetData(const OctetData& copy) : size(0)
{
	this->Initialize(copy.ToReadOnly());
}

openpal::ReadOnlyBuffer OctetData::ToReadOnly() const
{
	return ReadOnlyBuffer(pData, size);
}

}




