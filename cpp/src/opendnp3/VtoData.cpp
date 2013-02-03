
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "VtoData.h"

#include <assert.h>
#include <memory.h>

namespace opendnp3
{

std::string ToString(VtoDataType aType)
{
	switch(aType) {
	case(VTODT_DATA): return "DATA";
	case(VTODT_REMOTE_CLOSED): return "REMOTE_CLOSED";
	case(VTODT_REMOTE_OPENED): return "REMOTE_OPENED";
	default: return "Unknown VtoDataType";
	}
}

VtoData::VtoData() :
	mSize(0), mType(VTODT_DATA)
{}

VtoData::VtoData(size_t aSize) :
	mSize(aSize), mType(VTODT_DATA)
{
	assert(aSize <= MAX_SIZE);
}

VtoData::VtoData(VtoDataType aType) :
	mSize(0), mType(aType)
{}

VtoData::VtoData(const uint8_t* apValue, size_t aSize) :
	mType(VTODT_DATA)
{
	this->Copy(apValue, aSize);
}

size_t VtoData::GetSize() const
{
	return this->mSize;
}

VtoDataType VtoData::GetType() const
{
	return this->mType;
}

void VtoData::Copy(const uint8_t* apValue, size_t aSize)
{
	assert(aSize <= MAX_SIZE);
	memcpy(this->mpData, apValue, aSize);
	this->mSize = aSize;
}

}

/* vim: set ts=4 sw=4: */


