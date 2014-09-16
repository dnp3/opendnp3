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
#include "StaticLoadFunctions.h"

#include "opendnp3/objects/Group1.h"
#include "opendnp3/objects/Group3.h"
#include "opendnp3/objects/Group10.h"
#include "opendnp3/objects/Group20.h"
#include "opendnp3/objects/Group21.h"
#include "opendnp3/objects/Group30.h"
#include "opendnp3/objects/Group40.h"

using namespace openpal;

namespace opendnp3
{

StaticLoadFun StaticLoadFunctions::Get(StaticBinaryResponse rsp)
{
	switch(rsp)
	{
	case(StaticBinaryResponse::Group1Var2):
		return &StaticLoader::LoadFixedSizeStartStop<Group1Var2>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group1Var2>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticDoubleBinaryResponse rsp)
{
	switch (rsp)
	{
		case(StaticDoubleBinaryResponse::Group3Var2) :
			return &StaticLoader::LoadFixedSizeStartStop<Group3Var2>;
		default:
			return &StaticLoader::LoadFixedSizeStartStop<Group3Var2>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticAnalogResponse rsp)
{
	switch(rsp)
	{
	case(StaticAnalogResponse::Group30Var1): return &StaticLoader::LoadFixedSizeStartStop<Group30Var1>;
	case(StaticAnalogResponse::Group30Var2): return &StaticLoader::LoadFixedSizeStartStop<Group30Var2>;
	case(StaticAnalogResponse::Group30Var3): return &StaticLoader::LoadFixedSizeStartStop<Group30Var3>;
	case(StaticAnalogResponse::Group30Var4): return &StaticLoader::LoadFixedSizeStartStop<Group30Var4>;
	case(StaticAnalogResponse::Group30Var5): return &StaticLoader::LoadFixedSizeStartStop<Group30Var5>;
	case(StaticAnalogResponse::Group30Var6): return &StaticLoader::LoadFixedSizeStartStop<Group30Var6>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group30Var1>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticCounterResponse rsp)
{
	switch(rsp)
	{
	case(StaticCounterResponse::Group20Var1): return &StaticLoader::LoadFixedSizeStartStop<Group20Var1>;
	case(StaticCounterResponse::Group20Var2): return &StaticLoader::LoadFixedSizeStartStop<Group20Var2>;
	case(StaticCounterResponse::Group20Var5): return &StaticLoader::LoadFixedSizeStartStop<Group20Var5>;
	case(StaticCounterResponse::Group20Var6): return &StaticLoader::LoadFixedSizeStartStop<Group20Var6>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group20Var1>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticFrozenCounterResponse rsp)
{
	switch(rsp)
	{
	case(StaticFrozenCounterResponse::Group21Var1): return &StaticLoader::LoadFixedSizeStartStop<Group21Var1>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group21Var1>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticBinaryOutputStatusResponse rsp)
{
	switch(rsp)
	{
	case(StaticBinaryOutputStatusResponse::Group10Var2): return &StaticLoader::LoadFixedSizeStartStop<Group10Var2>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group10Var2>;
	}
}

StaticLoadFun StaticLoadFunctions::Get(StaticAnalogOutputStatusResponse rsp)
{
	switch(rsp)
	{
	case(StaticAnalogOutputStatusResponse::Group40Var1): return &StaticLoader::LoadFixedSizeStartStop<Group40Var1>;
	case(StaticAnalogOutputStatusResponse::Group40Var2): return &StaticLoader::LoadFixedSizeStartStop<Group40Var2>;
	case(StaticAnalogOutputStatusResponse::Group40Var3): return &StaticLoader::LoadFixedSizeStartStop<Group40Var3>;
	case(StaticAnalogOutputStatusResponse::Group40Var4): return &StaticLoader::LoadFixedSizeStartStop<Group40Var4>;
	default:
		return &StaticLoader::LoadFixedSizeStartStop<Group40Var1>;
	}
}





}

