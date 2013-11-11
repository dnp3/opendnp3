
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

#include "ObjectWriteIterator.h"

#include "PackingUnpacking.h"
#include "ObjectHeader.h"

#include <openpal/Exception.h>

namespace opendnp3
{

ObjectWriteIterator::ObjectWriteIterator() :
	mpPos(NULL),
	mIndex(1),
	mStart(0),
	mStop(0),
	mObjectSize(0)
{}


ObjectWriteIterator::ObjectWriteIterator(uint8_t* apPos, size_t aStart, size_t aStop, size_t aObjectSize) :
	mpPos(apPos),
	mIndex(aStart),
	mStart(aStart),
	mStop(aStop),
	mObjectSize(aObjectSize)
{

}

const ObjectWriteIterator& ObjectWriteIterator::operator++()
{
	if(this->IsEnd()) {
		MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "End of iterattion");
	}

	++mIndex;
	mpPos += mObjectSize;

	return (*this);
}

}

