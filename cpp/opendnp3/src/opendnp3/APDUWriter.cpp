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

#include "APDUWriter.h"

#include <openpal/Serialization.h>

#include <assert.h>

using namespace openpal;

namespace opendnp3
{

APDUWriter::APDUWriter(openpal::WriteBuffer aBuffer, uint32_t aStartSize) : 
	position(aBuffer),
	buffer(aBuffer)	
{
	assert(aStartSize <= aBuffer.Size());
	position.Advance(aStartSize);
}

void APDUWriter::SetFunction(FunctionCode code)
{
	buffer[1] = FunctionCodeToType(code);
}

void APDUWriter::SetControl(const AppControlField& control)
{
	buffer[0] = control.ToByte();
}

void APDUWriter::Mark()
{
	mark.Set(position);
}

void APDUWriter::Rollback()
{
	if(mark.IsSet()) position = mark.Get();		
}

bool APDUWriter::WriteHeader(GroupVariationID id, QualifierCode qc)
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

WriteBuffer APDUWriter::GetWritten() const
{
	auto size = buffer.Size() - position.Size();
	return buffer.Truncate(size);
}

APDURequestWriter::APDURequestWriter(openpal::WriteBuffer aBuffer) : APDUWriter(aBuffer, 2)
{}

APDUResponseWriter::APDUResponseWriter(openpal::WriteBuffer aBuffer) : APDUWriter(aBuffer, 4)
{}

void APDUResponseWriter::SetIIN(const IINField& indications)
{
	buffer[2] = indications.LSB;
	buffer[3] = indications.MSB;
}

}
