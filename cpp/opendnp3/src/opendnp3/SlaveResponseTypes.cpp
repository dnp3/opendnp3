
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

#include "SlaveResponseTypes.h"

#include <opendnp3/SlaveConfig.h>
#include <openpal/Exception.h>
#include <openpal/Location.h>

#include "Objects.h"

using namespace openpal;

namespace opendnp3
{

SlaveResponseTypes::SlaveResponseTypes(const SlaveConfig& arCfg)
{
	mpStaticBinary = GetStaticBinary(arCfg.mStaticBinary);
	mpStaticAnalog = GetStaticAnalog(arCfg.mStaticAnalog);
	mpStaticCounter = GetStaticCounter(arCfg.mStaticCounter);
	mpStaticControlStatus = Group10Var2::Inst();
	mpStaticSetpointStatus = GetStaticSetpointStatus(arCfg.mStaticSetpointStatus);

	mpEventBinary = GetEventBinary(arCfg.mEventBinary);
	mpEventAnalog = GetEventAnalog(arCfg.mEventAnalog);
	mpEventCounter = GetEventCounter(arCfg.mEventCounter);

	/* This is the only valid Slave VTO response, therefore it doesn't need to be configurable */
	mpEventVto = Group113Var0::Inst();
}

StreamObject<Binary>* SlaveResponseTypes::GetStaticBinary(StaticBinaryResponse rsp)
{
	switch(rsp) {
	case(SBR_GROUP1_VAR2):
		return Group1Var2::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static binary");
	}
}

StreamObject<Analog>* SlaveResponseTypes::GetStaticAnalog(StaticAnalogResponse rsp)
{
	switch(rsp) {
	case(SAR_GROUP30_VAR1): return Group30Var1::Inst();
	case(SAR_GROUP30_VAR2): return Group30Var2::Inst();
	case(SAR_GROUP30_VAR3): return Group30Var3::Inst();
	case(SAR_GROUP30_VAR4): return Group30Var4::Inst();
	case(SAR_GROUP30_VAR5): return Group30Var5::Inst();
	case(SAR_GROUP30_VAR6): return Group30Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static analog");
	}
}

StreamObject<Counter>* SlaveResponseTypes::GetStaticCounter(StaticCounterResponse rsp)
{
	switch(rsp) {
	case(SCommandResult::GROUP20_VAR1): return Group20Var1::Inst();
	case(SCommandResult::GROUP20_VAR2): return Group20Var2::Inst();
	case(SCommandResult::GROUP20_VAR5): return Group20Var5::Inst();
	case(SCommandResult::GROUP20_VAR6): return Group20Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static counter");
	}
}

StreamObject<SetpointStatus>* SlaveResponseTypes::GetStaticSetpointStatus(StaticSetpointStatusResponse rsp)
{
	switch(rsp) {
	case(SSSR_GROUP40_VAR1): return Group40Var1::Inst();
	case(SSSR_GROUP40_VAR2): return Group40Var2::Inst();
	case(SSSR_GROUP40_VAR3): return Group40Var3::Inst();
	case(SSSR_GROUP40_VAR4): return Group40Var4::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid static analog output status");
	}
}

StreamObject<Binary>* SlaveResponseTypes::GetEventBinary(EventBinaryResponse rsp)
{
	switch(rsp) {
	case(EBR_GROUP2_VAR1): return Group2Var1::Inst();
	case(EBR_GROUP2_VAR2): return Group2Var2::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event binary");
	}

}

StreamObject<Analog>* SlaveResponseTypes::GetEventAnalog(EventAnalogResponse rsp)
{
	switch(rsp) {
	case(EAR_GROUP32_VAR1): return Group32Var1::Inst();
	case(EAR_GROUP32_VAR2): return Group32Var2::Inst();
	case(EAR_GROUP32_VAR3): return Group32Var3::Inst();
	case(EAR_GROUP32_VAR4): return Group32Var4::Inst();
	case(EAR_GROUP32_VAR5): return Group32Var5::Inst();
	case(EAR_GROUP32_VAR6): return Group32Var6::Inst();
	case(EAR_GROUP32_VAR7): return Group32Var7::Inst();
	case(EAR_GROUP32_VAR8): return Group32Var8::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event analog");
	}
}

StreamObject<Counter>* SlaveResponseTypes::GetEventCounter(EventCounterResponse rsp)
{
	switch(rsp) {
	case(ECommandResult::GROUP22_VAR1): return Group22Var1::Inst();
	case(ECommandResult::GROUP22_VAR2): return Group22Var2::Inst();
	case(ECommandResult::GROUP22_VAR5): return Group22Var5::Inst();
	case(ECommandResult::GROUP22_VAR6): return Group22Var6::Inst();
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Invalid event counter");
	}
}

}

