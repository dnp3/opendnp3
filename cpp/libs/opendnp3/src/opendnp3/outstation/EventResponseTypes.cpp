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
			return &EventWriteFunctions::WriteFixedSize<Group2Var1>;
		case(EventBinaryResponse::Group2Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group2Var2>;
		case(EventBinaryResponse::Group2Var3) :
			return &EventWriteFunctions::WriteFixedSizeWithCTO<Group2Var3, Group51Var1>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group2Var1>;
		}
	}

	
	EventHeaderWriteFunc EventResponseTypes::Lookup(EventDoubleBinaryResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventDoubleBinaryResponse::Group4Var1) :
			return &EventWriteFunctions::WriteFixedSize<Group4Var1>;
		case(EventDoubleBinaryResponse::Group4Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group4Var2>;
		case(EventDoubleBinaryResponse::Group4Var3) :
			return &EventWriteFunctions::WriteFixedSizeWithCTO<Group4Var3, Group51Var1>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group4Var1>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventCounterResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventCounterResponse::Group22Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var1>;
			case(EventCounterResponse::Group22Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var2>;
			case(EventCounterResponse::Group22Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var5>;
			case(EventCounterResponse::Group22Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group22Var6>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group22Var1>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventFrozenCounterResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventFrozenCounterResponse::Group23Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var1>;
			case(EventFrozenCounterResponse::Group23Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var2>;
			case(EventFrozenCounterResponse::Group23Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var5>;
			case(EventFrozenCounterResponse::Group23Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group23Var6>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group23Var1>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventAnalogResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventAnalogResponse::Group32Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var1>;
			case(EventAnalogResponse::Group32Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var2>;
			case(EventAnalogResponse::Group32Var3) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var3>;
			case(EventAnalogResponse::Group32Var4) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var4>;
			case(EventAnalogResponse::Group32Var5) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var5>;
			case(EventAnalogResponse::Group32Var6) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var6>;
			case(EventAnalogResponse::Group32Var7) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var7>;
			case(EventAnalogResponse::Group32Var8) :
				return &EventWriteFunctions::WriteFixedSize<Group32Var8>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group32Var1>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventBinaryOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventBinaryOutputStatusResponse::Group11Var1) :
				return &EventWriteFunctions::WriteFixedSize<Group11Var1>;
			case(EventBinaryOutputStatusResponse::Group11Var2) :
				return &EventWriteFunctions::WriteFixedSize<Group11Var2>;
			default:
				return &EventWriteFunctions::WriteFixedSize<Group11Var1>;
		}
	}

	EventHeaderWriteFunc EventResponseTypes::Lookup(EventAnalogOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventAnalogOutputStatusResponse::Group42Var1) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var1>;
		case(EventAnalogOutputStatusResponse::Group42Var2) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var2>;
		case(EventAnalogOutputStatusResponse::Group42Var3) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var3>;
		case(EventAnalogOutputStatusResponse::Group42Var4) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var4>;
		case(EventAnalogOutputStatusResponse::Group42Var5) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var5>;
		case(EventAnalogOutputStatusResponse::Group42Var6) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var6>;
		case(EventAnalogOutputStatusResponse::Group42Var7) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var7>;
		case(EventAnalogOutputStatusResponse::Group42Var8) :
			return &EventWriteFunctions::WriteFixedSize<Group42Var8>;
		default:
			return &EventWriteFunctions::WriteFixedSize<Group42Var1>;
		}
	}
	

}
