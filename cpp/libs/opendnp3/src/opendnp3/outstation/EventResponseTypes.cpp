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

#include "EventResponseTypes.h"

#include "opendnp3/objects/Group2.h"
#include "opendnp3/objects/Group4.h"
#include "opendnp3/objects/Group11.h"
#include "opendnp3/objects/Group22.h"
#include "opendnp3/objects/Group23.h"
#include "opendnp3/objects/Group32.h"
#include "opendnp3/objects/Group42.h"
#include "opendnp3/objects/Group51.h"

#include "EventWriteFunctions.h"


namespace opendnp3
{	
	EventHeaderWriteFunc EventResponseTypes::Lookup(EventBinaryResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventBinaryResponse::Group2Var1) :
			return &EventWriteFunctions::WriteFixedSize<Group2Var1Serializer>;
		case(EventBinaryResponse::Group2Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group2Var2Serializer>;
		case(EventBinaryResponse::Group2Var3) :
			return &EventWriteFunctions::WriteFixedSizeWithCTO<Group2Var3Serializer, Group51Var1>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group2Var1Serializer>;
		}
	}

	
	EventHeaderWriteFunc EventResponseTypes::Lookup(EventDoubleBinaryResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventDoubleBinaryResponse::Group4Var1) :
			return &EventWriteFunctions::WriteFixedSize<Group4Var1Serializer>;
		case(EventDoubleBinaryResponse::Group4Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group4Var2Serializer>;
		case(EventDoubleBinaryResponse::Group4Var3) :
			return &EventWriteFunctions::WriteFixedSizeWithCTO<Group4Var3Serializer, Group51Var1>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group4Var1Serializer>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventCounterResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventCounterResponse::Group22Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var1Serializer>;
			case(EventCounterResponse::Group22Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var2Serializer>;
			case(EventCounterResponse::Group22Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var5Serializer>;
			case(EventCounterResponse::Group22Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var6Serializer>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group22Var1Serializer>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventFrozenCounterResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventFrozenCounterResponse::Group23Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var1Serializer>;
			case(EventFrozenCounterResponse::Group23Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var2Serializer>;
			case(EventFrozenCounterResponse::Group23Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var5Serializer>;
			case(EventFrozenCounterResponse::Group23Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var6Serializer>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group23Var1Serializer>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventAnalogResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventAnalogResponse::Group32Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var1Serializer>;
			case(EventAnalogResponse::Group32Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var2Serializer>;
			case(EventAnalogResponse::Group32Var3) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var3Serializer>;
			case(EventAnalogResponse::Group32Var4) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var4Serializer>;
			case(EventAnalogResponse::Group32Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var5Serializer>;
			case(EventAnalogResponse::Group32Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var6Serializer>;
			case(EventAnalogResponse::Group32Var7) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var7Serializer>;
			case(EventAnalogResponse::Group32Var8) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var8Serializer>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group32Var1Serializer>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventBinaryOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventBinaryOutputStatusResponse::Group11Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group11Var1Serializer>;
			case(EventBinaryOutputStatusResponse::Group11Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group11Var2Serializer>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group11Var1Serializer>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventAnalogOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventAnalogOutputStatusResponse::Group42Var1) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var1Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var2Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var3) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var3Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var4) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var4Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var5) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var5Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var6) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var6Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var7) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var7Serializer>;
		case(EventAnalogOutputStatusResponse::Group42Var8) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var8Serializer>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group42Var1Serializer>;
		}
	}
	

}
