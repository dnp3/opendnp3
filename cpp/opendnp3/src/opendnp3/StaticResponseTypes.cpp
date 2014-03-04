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
#include "StaticResponseTypes.h"

#include "SlaveConfig.h"

#include "objects/Group1.h"
#include "objects/Group10.h"
#include "objects/Group20.h"
#include "objects/Group21.h"
#include "objects/Group30.h"
#include "objects/Group40.h"

using namespace openpal;

namespace opendnp3
{

StaticResponseTypes::StaticResponseTypes(const SlaveConfig& arCfg) :
	pStaticBinaryLoader(GetStaticBinary(arCfg.mStaticBinary)),
	pStaticAnalogLoader(GetStaticAnalog(arCfg.mStaticAnalog)),
	pStaticCounterLoader(GetStaticCounter(arCfg.mStaticCounter)),
	pStaticFrozenCounterLoader(GetStaticFrozenCounter(arCfg.mStaticFrozenCounter)),
	pStaticBinaryOutputStatusLoader(GetStaticBinaryOutputStatus(arCfg.mStaticBinaryOutputStatus)),
	pStaticAnalogOutputStatusLoader(GetStaticAnalogOutputStatus(arCfg.mStaticAnalogOutputStatus))
{
		
}

StaticResponseTypes::StaticResponseTypes() :
	pStaticBinaryLoader(StaticLoader::GetLoadFunction<Group1Var2Serializer>()),
	pStaticAnalogLoader(StaticLoader::GetLoadFunction<Group30Var1Serializer>()),
	pStaticCounterLoader(StaticLoader::GetLoadFunction<Group20Var1Serializer>()),
	pStaticFrozenCounterLoader(StaticLoader::GetLoadFunction<Group21Var1Serializer>()),
	pStaticBinaryOutputStatusLoader(StaticLoader::GetLoadFunction<Group10Var2Serializer>()),
	pStaticAnalogOutputStatusLoader(StaticLoader::GetLoadFunction<Group40Var1Serializer>())
{
		
}

template <>
StaticLoadFun StaticResponseTypes::GetLoader<Binary>() { return pStaticBinaryLoader; }

template <>
StaticLoadFun StaticResponseTypes::GetLoader<Analog>() { return pStaticAnalogLoader; }

template <>
StaticLoadFun StaticResponseTypes::GetLoader<Counter>() { return pStaticCounterLoader; }

template <>
StaticLoadFun StaticResponseTypes::GetLoader<FrozenCounter>() { return pStaticFrozenCounterLoader; }

template <>
StaticLoadFun StaticResponseTypes::GetLoader<BinaryOutputStatus>() { return pStaticBinaryOutputStatusLoader; }

template <>
StaticLoadFun StaticResponseTypes::GetLoader<AnalogOutputStatus>() { return pStaticAnalogOutputStatusLoader; }

StaticLoadFun StaticResponseTypes::GetStaticBinary(StaticBinaryResponse rsp)
{
	switch(rsp) {
		case(StaticBinaryResponse::Group1Var2):
			return StaticLoader::GetLoadFunction<Group1Var2Serializer>();
		default:
			return StaticLoader::GetLoadFunction<Group1Var2Serializer>();
	}
}

StaticLoadFun StaticResponseTypes::GetStaticAnalog(StaticAnalogResponse rsp)
{
	switch(rsp) {
		case(StaticAnalogResponse::Group30Var1): return StaticLoader::GetLoadFunction<Group30Var1Serializer>();
		case(StaticAnalogResponse::Group30Var2): return StaticLoader::GetLoadFunction<Group30Var2Serializer>();
		case(StaticAnalogResponse::Group30Var3): return StaticLoader::GetLoadFunction<Group30Var3Serializer>();
		case(StaticAnalogResponse::Group30Var4): return StaticLoader::GetLoadFunction<Group30Var4Serializer>();
		case(StaticAnalogResponse::Group30Var5): return StaticLoader::GetLoadFunction<Group30Var5Serializer>();
		case(StaticAnalogResponse::Group30Var6): return StaticLoader::GetLoadFunction<Group30Var6Serializer>();
	default:
		return StaticLoader::GetLoadFunction<Group30Var1Serializer>();
	}
}

StaticLoadFun StaticResponseTypes::GetStaticCounter(StaticCounterResponse rsp)
{
	switch(rsp) {
		case(StaticCounterResponse::Group20Var1): return StaticLoader::GetLoadFunction<Group20Var1Serializer>();
		case(StaticCounterResponse::Group20Var2): return StaticLoader::GetLoadFunction<Group20Var2Serializer>();
		case(StaticCounterResponse::Group20Var5): return StaticLoader::GetLoadFunction<Group20Var5Serializer>();
		case(StaticCounterResponse::Group20Var6): return StaticLoader::GetLoadFunction<Group20Var6Serializer>();
	default:
		return StaticLoader::GetLoadFunction<Group20Var1Serializer>();
	}
}

StaticLoadFun StaticResponseTypes::GetStaticFrozenCounter(StaticFrozenCounterResponse rsp)
{
	switch(rsp) {
		case(StaticFrozenCounterResponse::Group21Var1): return StaticLoader::GetLoadFunction<Group21Var1Serializer>();
	default:
		return StaticLoader::GetLoadFunction<Group21Var1Serializer>();
	}
}

StaticLoadFun StaticResponseTypes::GetStaticBinaryOutputStatus(StaticBinaryOutputStatusResponse rsp)
{
	switch(rsp) {
	case(StaticBinaryOutputStatusResponse::Group10Var2): return StaticLoader::GetLoadFunction<Group10Var2Serializer>();		
	default:
		return StaticLoader::GetLoadFunction<Group10Var2Serializer>();
	}
}

StaticLoadFun StaticResponseTypes::GetStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse rsp)
{
	switch(rsp) {
		case(StaticAnalogOutputStatusResponse::Group40Var1): return StaticLoader::GetLoadFunction<Group40Var1Serializer>();
		case(StaticAnalogOutputStatusResponse::Group40Var2): return StaticLoader::GetLoadFunction<Group40Var2Serializer>();
		case(StaticAnalogOutputStatusResponse::Group40Var3): return StaticLoader::GetLoadFunction<Group40Var3Serializer>();
		case(StaticAnalogOutputStatusResponse::Group40Var4): return StaticLoader::GetLoadFunction<Group40Var4Serializer>();
	default:
		return StaticLoader::GetLoadFunction<Group40Var1Serializer>();
	}
}





}

