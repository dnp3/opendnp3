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


namespace opendnp3
{	
	IDNP3Serializer<Binary>* EventResponseTypes::Lookup(EventBinaryResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventBinaryResponse::Group2Var1):
				return Group2Var1Serializer::Inst();
			case(EventBinaryResponse::Group2Var2):
				return Group2Var2Serializer::Inst();
			default:
				return Group2Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<DoubleBitBinary>* EventResponseTypes::Lookup(EventDoubleBinaryResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventDoubleBinaryResponse::Group4Var1) :
				return Group4Var1Serializer::Inst();
			case(EventDoubleBinaryResponse::Group4Var2) :
				return Group4Var2Serializer::Inst();
			default:
				return Group4Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<Counter>* EventResponseTypes::Lookup(EventCounterResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventCounterResponse::Group22Var1) :
				return Group22Var1Serializer::Inst();
			case(EventCounterResponse::Group22Var2) :
				return Group22Var2Serializer::Inst();
			case(EventCounterResponse::Group22Var5) :
				return Group22Var5Serializer::Inst();
			case(EventCounterResponse::Group22Var6) :
				return Group22Var6Serializer::Inst();
			default:
				return Group22Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<FrozenCounter>* EventResponseTypes::Lookup(EventFrozenCounterResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventFrozenCounterResponse::Group23Var1) :
				return Group23Var1Serializer::Inst();
			case(EventFrozenCounterResponse::Group23Var2) :
				return Group23Var2Serializer::Inst();
			case(EventFrozenCounterResponse::Group23Var5) :
				return Group23Var5Serializer::Inst();
			case(EventFrozenCounterResponse::Group23Var6) :
				return Group23Var6Serializer::Inst();
			default:
				return Group23Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<Analog>* EventResponseTypes::Lookup(EventAnalogResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventAnalogResponse::Group32Var1) :
				return Group32Var1Serializer::Inst();
			case(EventAnalogResponse::Group32Var2) :
				return Group32Var2Serializer::Inst();
			case(EventAnalogResponse::Group32Var3) :
				return Group32Var3Serializer::Inst();
			case(EventAnalogResponse::Group32Var4) :
				return Group32Var4Serializer::Inst();
			case(EventAnalogResponse::Group32Var5) :
				return Group32Var5Serializer::Inst();
			case(EventAnalogResponse::Group32Var6) :
				return Group32Var6Serializer::Inst();
			case(EventAnalogResponse::Group32Var7) :
				return Group32Var7Serializer::Inst();
			case(EventAnalogResponse::Group32Var8) :
				return Group32Var8Serializer::Inst();
			default:
				return Group32Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<BinaryOutputStatus>* EventResponseTypes::Lookup(EventBinaryOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
			case(EventBinaryOutputStatusResponse::Group11Var1) :
				return Group11Var1Serializer::Inst();
			case(EventBinaryOutputStatusResponse::Group11Var2) :
				return Group11Var2Serializer::Inst();
			default:
				return Group11Var1Serializer::Inst();
		}
	}

	IDNP3Serializer<AnalogOutputStatus>* EventResponseTypes::Lookup(EventAnalogOutputStatusResponse enumeration)
	{
		switch (enumeration)
		{
		case(EventAnalogOutputStatusResponse::Group42Var1) :
			return Group42Var1Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var2) :
			return Group42Var2Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var3) :
			return Group42Var3Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var4) :
			return Group42Var4Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var5) :
			return Group42Var5Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var6) :
			return Group42Var6Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var7) :
			return Group42Var7Serializer::Inst();
		case(EventAnalogOutputStatusResponse::Group42Var8) :
			return Group42Var8Serializer::Inst();
		default:
			return Group42Var1Serializer::Inst();
		}
	}

}
