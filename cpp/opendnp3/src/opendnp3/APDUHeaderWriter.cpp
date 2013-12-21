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

#include "APDUHeaderWriter.h"

#include <openpal/Serialization.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

APDUHeaderWriter::APDUHeaderWriter(openpal::WriteBuffer aBuffer) : 
	buffer(aBuffer),
	position(aBuffer)	
{
		
}

void APDUHeaderWriter::Mark()
{
	mark.Set(position);
}

void APDUHeaderWriter::Rollback()
{
	if(mark.IsSet()) position = mark.Get();		
}

bool APDUHeaderWriter::WriteHeader(GroupVariationID id, QualifierCode qc)
{
	if(position.Size() >= 3)
	{
		UInt8::WriteBuffer(position, id.group);
		UInt8::WriteBuffer(position, id.variation);
		UInt8::WriteBuffer(position, QualifierCodeToType(qc));
		return true;
	}
	else return false;
}

uint32_t APDUHeaderWriter::Size() const
{
	return buffer.Size() - position.Size();	
}

ReadOnlyBuffer APDUHeaderWriter::ToReadOnly() const
{
	auto size = buffer.Size() - position.Size();
	return ReadOnlyBuffer(buffer, size);
}

}
