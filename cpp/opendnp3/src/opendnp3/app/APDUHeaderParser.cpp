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

#include "APDUHeaderParser.h"

#include <functional>
#include <limits>

#include <openpal/Uncopyable.h>
#include "opendnp3/app/APDUHeader.h"

#include <openpal/BufferWrapper.h>


namespace opendnp3
{

APDUHeaderParser::Result APDUHeaderParser::ParseRequest(openpal::ReadOnlyBuffer buffer, APDURecord& header)
{
	if(buffer.Size() < 2) return Result::NOT_ENOUGH_DATA_FOR_HEADER;
	else
	{
		header.control = AppControlField(buffer[0]);
		header.function = FunctionCodeFromType(buffer[1]);
		buffer.Advance(2);
		header.objects = buffer;
		return Result::OK;
	}
}

APDUHeaderParser::Result APDUHeaderParser::ParseResponse(openpal::ReadOnlyBuffer buffer, APDUResponseRecord& header)
{
	if(buffer.Size() < 4) return Result::NOT_ENOUGH_DATA_FOR_HEADER;
	else
	{
		header.control = AppControlField(buffer[0]);
		header.function = FunctionCodeFromType(buffer[1]);
		header.IIN.LSB = buffer[2];
		header.IIN.MSB = buffer[3];
		buffer.Advance(4);
		header.objects = buffer;
		return Result::OK;
	}
}

}

