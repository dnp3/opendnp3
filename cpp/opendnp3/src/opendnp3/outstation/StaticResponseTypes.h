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
#ifndef __STATIC_RESPONSE_TYPES_H_
#define __STATIC_RESPONSE_TYPES_H_

#include "opendnp3/app/DataTypes.h"

#include "opendnp3/outstation/SlaveConfig.h"
#include "opendnp3/outstation/StaticLoader.h"

namespace opendnp3
{

/**
 * Reads a slave config object and and translates the configuration to
 * singletons.
 */
class StaticResponseTypes
{
public:

	StaticResponseTypes();

	StaticResponseTypes(const SlaveConfig& arCfg);
			
	template <class T>
	StaticLoadFun GetLoader();

private:

	const StaticLoadFun pStaticBinaryLoader;
	const StaticLoadFun pStaticAnalogLoader;
	const StaticLoadFun pStaticCounterLoader;
	const StaticLoadFun pStaticFrozenCounterLoader;
	const StaticLoadFun pStaticBinaryOutputStatusLoader;
	const StaticLoadFun pStaticAnalogOutputStatusLoader;

	static StaticLoadFun GetStaticBinary(StaticBinaryResponse rsp);
	static StaticLoadFun GetStaticCounter(StaticCounterResponse rsp);	
	static StaticLoadFun GetStaticFrozenCounter(StaticFrozenCounterResponse rsp);
	static StaticLoadFun GetStaticAnalog(StaticAnalogResponse rsp);	
	static StaticLoadFun GetStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse rsp);	
	static StaticLoadFun GetStaticBinaryOutputStatus(StaticBinaryOutputStatusResponse rsp);	
};

}

#endif

