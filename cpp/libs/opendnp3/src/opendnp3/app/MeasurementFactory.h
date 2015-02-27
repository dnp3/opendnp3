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
#ifndef OPENDNP3_MEASUREMENTFACTORY_H
#define OPENDNP3_MEASUREMENTFACTORY_H

#include <openpal/util/Uncopyable.h>
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/BinaryCommandEvent.h"
#include "opendnp3/app/AnalogCommandEvent.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3
{

struct BinaryFactory: private openpal::PureStatic
{
	inline static Binary From(uint8_t flags)
	{
		return Binary(flags);
	}

	inline static Binary From(uint8_t flags, uint16_t time)
	{
		return From(flags, DNPTime(time));
	}

	inline static Binary From(uint8_t flags, DNPTime time)
	{
		return Binary(flags, time);
	}
};

struct DoubleBitBinaryFactory : private openpal::PureStatic
{
	inline static DoubleBitBinary From(uint8_t flags)
	{
		return DoubleBitBinary(flags);
	}

	inline static DoubleBitBinary From(uint8_t flags, uint16_t time)
	{
		return From(flags, DNPTime(time));
	}

	inline static DoubleBitBinary From(uint8_t flags, DNPTime time)
	{
		return DoubleBitBinary(flags, time);
	}
};

struct AnalogFactory: private openpal::PureStatic
{
	static Analog From(int32_t count)
	{
		return Analog(count);
	}
	static Analog From(uint8_t flags, double value)
	{
		return Analog(value, flags);
	}
	static Analog From(uint8_t flags, double value, DNPTime time)
	{
		return Analog(value, flags, time);
	}
};

struct CounterFactory: private openpal::PureStatic
{
	inline static Counter From(uint32_t count)
	{
		return Counter(count);
	}
	inline static Counter From(uint8_t flags, uint32_t count)
	{
		return Counter(count, flags);
	}
	inline static Counter From(uint8_t flags, uint32_t count, DNPTime time)
	{
		return Counter(count, flags, time);
	}
};

struct FrozenCounterFactory: private openpal::PureStatic
{
	inline static FrozenCounter From(uint32_t count)
	{
		return FrozenCounter(count);
	}
	inline static FrozenCounter From(uint8_t flags, uint32_t count)
	{
		return FrozenCounter(count, flags);
	}
	inline static FrozenCounter From(uint8_t flags, uint32_t count, DNPTime time)
	{
		return FrozenCounter(count, flags, time);
	}
};

struct TimeAndIntervalFactory : private openpal::PureStatic
{	
	inline static TimeAndInterval From(DNPTime time, uint32_t interval, uint8_t units)
	{
		return TimeAndInterval(time, interval, units);
	}
};

struct ControlRelayOutputBlockFactory: private openpal::PureStatic
{
	inline static ControlRelayOutputBlock From(
	    uint8_t code,
	    uint8_t count,
	    uint32_t onTime,
	    uint32_t offTime,
	    CommandStatus status)
	{		
		return ControlRelayOutputBlock(code, count, onTime, offTime, status);
	}
};

struct BinaryOutputStatusFactory: private openpal::PureStatic
{
	inline static BinaryOutputStatus From(uint8_t flags)
	{
		return BinaryOutputStatus(flags);
	}

	inline static BinaryOutputStatus From(uint8_t flags, DNPTime time)
	{
		return BinaryOutputStatus(flags, time);
	}
};

struct BinaryCommandEventFactory : private openpal::PureStatic
{
	inline static BinaryCommandEvent From(uint8_t flags)
	{
		return BinaryCommandEvent(flags);
	}

	inline static BinaryCommandEvent From(uint8_t flags, DNPTime time)
	{
		return BinaryCommandEvent(flags, time);
	}
};

struct AnalogOutputStatusFactory: private openpal::PureStatic
{
	inline static AnalogOutputStatus From(uint8_t flags, double value)
	{
		return AnalogOutputStatus(value, flags);
	}

	inline static AnalogOutputStatus From(uint8_t flags, double value, DNPTime time)
	{
		return AnalogOutputStatus(value, flags, time);
	}
};

struct AnalogCommandEventFactory : private openpal::PureStatic
{
	inline static AnalogCommandEvent From(CommandStatus status, double value)
	{
		return AnalogCommandEvent(value, status);
	}

	inline static AnalogCommandEvent From(CommandStatus status, double value, DNPTime time)
	{
		return AnalogCommandEvent(value, status, time);
	}
};

template <class Target, class ValueType>
struct AnalogOutputFactory: private openpal::PureStatic
{
	inline static Target From(ValueType value, CommandStatus status)
	{
		return Target(value, status);
	}
};

typedef AnalogOutputFactory<AnalogOutputInt32, int32_t> AnalogOutputInt32Factory;
typedef AnalogOutputFactory<AnalogOutputInt16, int16_t> AnalogOutputInt16Factory;
typedef AnalogOutputFactory<AnalogOutputFloat32, float> AnalogOutputFloat32Factory;
typedef AnalogOutputFactory<AnalogOutputDouble64, double> AnalogOutputDouble64Factory;



}

#endif
