
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

#include "OctetData.h"

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
	mSize = aSize;
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

size_t OctetData::GetSize() const
{
	return this->mSize;
}


}

/* vim: set ts=4 sw=4: */


