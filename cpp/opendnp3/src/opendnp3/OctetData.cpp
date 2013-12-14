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
#include <opendnp3/OctetData.h>

#include <assert.h>
#include <memory.h>

using namespace openpal;

namespace opendnp3
{

OctetData::OctetData() :  mpData(nullptr), mSize(0)
{

}

OctetData::OctetData(const openpal::ReadOnlyBuffer& buffer)
{
	Initialize(buffer);
}

void OctetData::Initialize(const openpal::ReadOnlyBuffer& buffer)
{
	assert(buffer.Size() <= MAX_SIZE);
	mpData = new uint8_t[buffer.Size()];
	buffer.CopyTo(mpData);
	mSize = buffer.Size();
}

OctetData& OctetData::operator=( const OctetData& rhs )
{
	if(&rhs != this) {		
		assert(rhs.mSize <= MAX_SIZE);
		if(mpData != nullptr)
		{
			delete[] mpData;
			mpData = nullptr;
			mSize = 0;
		}
		this->Initialize(rhs.ToReadOnly());
	}
	return *this;
}


OctetData::OctetData(const OctetData& aCopy) : mpData(nullptr), mSize(0)
{	
	this->Initialize(aCopy.ToReadOnly());
}

OctetData::~OctetData()
{
	if(mpData != nullptr) {
		delete[] mpData;
		mpData = nullptr;
	}
}

openpal::ReadOnlyBuffer OctetData::ToReadOnly() const
{
	return ReadOnlyBuffer(mpData, mSize);
}

std::string OctetData::AsString() const
{
	return std::string(reinterpret_cast<char*>(mpData), mSize);
}

}

/* vim: set ts=4 sw=4: */


