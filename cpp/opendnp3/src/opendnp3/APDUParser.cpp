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

#include "APDUParser.h"

#include <openpal/Serialization.h>

#include "gen/QualifierCode.h"
#include "GroupVariation.h"

using namespace openpal;

namespace opendnp3
{

APDUParser::Result APDUParser::ParseHeaders(ReadOnlyBuffer aBuffer, IAPDUHeaderHandler& arHandler)
{
	while(aBuffer.Size() > 0)
	{
		auto result = ParseHeader(aBuffer, arHandler);
		if(result != APDUParser::Result::OK) return result;
	}

	return APDUParser::Result::OK;
}

APDUParser::Result APDUParser::ParseHeader(ReadOnlyBuffer& arBuffer, IAPDUHeaderHandler& arHandler)
{
	if(arBuffer.Size() < 3) return APDUParser::Result::NOT_ENOUGH_DATA_FOR_HEADER; 
	else {
		uint8_t group = UInt8::ReadBuffer(arBuffer);		
		uint8_t variation = UInt8::ReadBuffer(arBuffer);		
		auto gv = GroupVariationDescriptor::GetEnum(group, variation);
		if(gv == GroupVariation::UNKNOWN) return APDUParser::Result::UNKNOWN_OBJECT;
		QualifierCode qualifier = QualifierCodeFromType(UInt8::ReadBuffer(arBuffer));
		switch(qualifier)
		{
			case(QualifierCode::ALL_OBJECTS):
				arHandler.AllObjects(gv);
				return APDUParser::Result::OK;
			default:
				return APDUParser::Result::UNKNOWN_QUALIFIER;
		}
	}
}

}


