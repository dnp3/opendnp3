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
#include "SlaveResponseTypes.h"

#include <opendnp3/SlaveConfig.h>
#include <openpal/Exception.h>
#include <openpal/Location.h>

using namespace openpal;

namespace opendnp3
{

SlaveResponseTypes::SlaveResponseTypes(const SlaveConfig& arCfg)
{
	/*
	mpStaticBinary = GetStaticBinary(arCfg.mStaticBinary);
	mpStaticAnalog = GetStaticAnalog(arCfg.mStaticAnalog);
	mpStaticCounter = GetStaticCounter(arCfg.mStaticCounter);
	mpStaticControlStatus = Group10Var2::Inst();
	mpStaticSetpointStatus = GetStaticSetpointStatus(arCfg.mStaticSetpointStatus);

	mpEventBinary = GetEventBinary(arCfg.mEventBinary);
	mpEventAnalog = GetEventAnalog(arCfg.mEventAnalog);
	mpEventCounter = GetEventCounter(arCfg.mEventCounter);

	*/
}

/*
StreamObject<Binary>* SlaveResponseTypes::GetStaticBinary(StaticBinaryResponse rsp)
{
	switch(rsp) {
	case(StaticBinaryResponse::Group1Var2):
		return Group1Var2::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static binary");
	}
}

StreamObject<Analog>* SlaveResponseTypes::GetStaticAnalog(StaticAnalogResponse rsp)
{
	switch(rsp) {
	case(StaticAnalogResponse::Group30Var1): return Group30Var1::Inst();
	case(StaticAnalogResponse::Group30Var2): return Group30Var2::Inst();
	case(StaticAnalogResponse::Group30Var3): return Group30Var3::Inst();
	case(StaticAnalogResponse::Group30Var4): return Group30Var4::Inst();
	case(StaticAnalogResponse::Group30Var5): return Group30Var5::Inst();
	case(StaticAnalogResponse::Group30Var6): return Group30Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static analog");
	}
}

StreamObject<Counter>* SlaveResponseTypes::GetStaticCounter(StaticCounterResponse rsp)
{
	switch(rsp) {
	case(StaticCounterResponse::Group20Var1): return Group20Var1::Inst();
	case(StaticCounterResponse::Group20Var2): return Group20Var2::Inst();
	case(StaticCounterResponse::Group20Var5): return Group20Var5::Inst();
	case(StaticCounterResponse::Group20Var6): return Group20Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static counter");
	}
}

StreamObject<SetpointStatus>* SlaveResponseTypes::GetStaticSetpointStatus(StaticSetpointStatusResponse rsp)
{
	switch(rsp) {
	case(StaticSetpointStatusResponse::Group40Var1): return Group40Var1::Inst();
	case(StaticSetpointStatusResponse::Group40Var2): return Group40Var2::Inst();
	case(StaticSetpointStatusResponse::Group40Var3): return Group40Var3::Inst();
	case(StaticSetpointStatusResponse::Group40Var4): return Group40Var4::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static analog output status");
	}
}

StreamObject<Binary>* SlaveResponseTypes::GetEventBinary(EventBinaryResponse rsp)
{
	switch(rsp) {
	case(EventBinaryResponse::Group2Var1): return Group2Var1::Inst();
	case(EventBinaryResponse::Group2Var2): return Group2Var2::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event binary");
	}

}

StreamObject<Analog>* SlaveResponseTypes::GetEventAnalog(EventAnalogResponse rsp)
{
	switch(rsp) {
	case(EventAnalogResponse::Group32Var1): return Group32Var1::Inst();
	case(EventAnalogResponse::Group32Var2): return Group32Var2::Inst();
	case(EventAnalogResponse::Group32Var3): return Group32Var3::Inst();
	case(EventAnalogResponse::Group32Var4): return Group32Var4::Inst();
	case(EventAnalogResponse::Group32Var5): return Group32Var5::Inst();
	case(EventAnalogResponse::Group32Var6): return Group32Var6::Inst();
	case(EventAnalogResponse::Group32Var7): return Group32Var7::Inst();
	case(EventAnalogResponse::Group32Var8): return Group32Var8::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event analog");
	}
}

StreamObject<Counter>* SlaveResponseTypes::GetEventCounter(EventCounterResponse rsp)
{
	switch(rsp) {
	case(EventCounterResponse::Group22Var1): return Group22Var1::Inst();
	case(EventCounterResponse::Group22Var2): return Group22Var2::Inst();
	case(EventCounterResponse::Group22Var5): return Group22Var5::Inst();
	case(EventCounterResponse::Group22Var6): return Group22Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event counter");
	}
}
*/

}

