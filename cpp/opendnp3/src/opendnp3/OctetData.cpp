
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

#include <opendnp3/OctetData.h>

#include <assert.h>
#include <memory.h>

namespace opendnp3
{

OctetData::OctetData() :  mpData(NULL), mSize(0)
{

}

OctetData::OctetData(const uint8_t* apValue, size_t aSize) : mpData(NULL), mSize(0)	
{
	assert(aSize <= MAX_SIZE);
	mpData = new uint8_t[aSize];
	memcpy(mpData, apValue, aSize);
	mSize = aSize;
}

OctetData& OctetData::operator=( const OctetData& rhs )
{
	if(&rhs != this) {
		assert(rhs.mSize <= MAX_SIZE);
		if(mpData != NULL)
		{
			delete[] mpData;
			mpData = NULL;
			mSize = 0;
		}
		mpData = new uint8_t[rhs.mSize];
		memcpy(mpData, rhs.mpData, rhs.mSize);
		mSize = rhs.mSize;
	}
	return *this;
}


OctetData::OctetData(const OctetData& arCopy) : mpData(NULL), mSize(0)
{
	mSize = arCopy.GetSize();
	mpData = new uint8_t[mSize];
	memcpy(mpData, arCopy.Data(), mSize);	
}

OctetData::~OctetData()
{
	if(mpData != NULL) {
		delete[] mpData;
		mpData = NULL;
	}
}

const uint8_t* OctetData::Data() const
{
	return mpData;
}

std::string OctetData::AsString() const
{
	return std::string(reinterpret_cast<char*>(mpData), mSize);
}

size_t OctetData::GetSize() const
{
	return this->mSize;
}


}

/* vim: set ts=4 sw=4: */


