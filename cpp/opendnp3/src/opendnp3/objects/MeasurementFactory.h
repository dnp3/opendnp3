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
#ifndef __MEASUREMENT_FACTORY_H_
#define __MEASUREMENT_FACTORY_H_

#include <opendnp3/Uncopyable.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/ControlRelayOutputBlock.h>

namespace opendnp3
{

struct BinaryFactory: private PureStatic
{
	inline static Binary From(uint8_t aFlags) { return Binary(aFlags); }
	inline static Binary From(uint8_t aFlags, int64_t aTime) { return Binary((aFlags & BQ_STATE) != 0, aFlags, aTime); }
};

struct AnalogFactory: private PureStatic
{
	static Analog From(int32_t aCount) { return Analog(aCount); }
	static Analog From(uint8_t aFlags, double aValue) { return Analog(aValue, aFlags); }
	static Analog From(uint8_t aFlags, double aValue, int64_t aTime) { return Analog(aValue, aFlags, aTime); }
};

struct CounterFactory: private PureStatic
{
	inline static Counter From(uint32_t aCount) { return Counter(aCount); }
	inline static Counter From(uint8_t aFlags, uint32_t aCount) { return Counter(aCount, aFlags); }
	inline static Counter From(uint8_t aFlags, uint32_t aCount, int64_t aTime) { return Counter(aCount, aFlags, aTime); }
};

struct ControlRelayOutputBlockFactory: private PureStatic
{
	inline static ControlRelayOutputBlock From(
		ControlCode aCode, 
		uint8_t aCount, 
		uint32_t aOnTime, 
		uint32_t aOffTime, 
		CommandStatus aStatus)
	{ 
		return ControlRelayOutputBlock(aCode, aCount, aOnTime, aOffTime, aStatus);
	}
};

struct ControlStatusFactory: private PureStatic
{
	inline static ControlStatus From(uint8_t flags) { return ControlStatus(flags); }
};


struct SetpointStatusFactory: private PureStatic
{
	inline static SetpointStatus From(uint8_t flags, double value) { return SetpointStatus(value, flags); }
};

}

#endif
