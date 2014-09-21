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
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3
{

struct BinaryFactory: private openpal::PureStatic
{
	inline static Binary From(uint8_t aFlags)
	{
		return Binary(aFlags);
	}
	inline static Binary From(uint8_t aFlags, int64_t aTime)
	{
		return Binary(aFlags, aTime);
	}
};

struct DoubleBitBinaryFactory : private openpal::PureStatic
{
	inline static DoubleBitBinary From(uint8_t aFlags)
	{
		return DoubleBitBinary(aFlags);
	}
	inline static DoubleBitBinary From(uint8_t aFlags, int64_t aTime)
	{
		return DoubleBitBinary(aFlags, aTime);
	}
};

struct AnalogFactory: private openpal::PureStatic
{
	static Analog From(int32_t aCount)
	{
		return Analog(aCount);
	}
	static Analog From(uint8_t aFlags, double aValue)
	{
		return Analog(aValue, aFlags);
	}
	static Analog From(uint8_t aFlags, double aValue, int64_t aTime)
	{
		return Analog(aValue, aFlags, aTime);
	}
};

struct CounterFactory: private openpal::PureStatic
{
	inline static Counter From(uint32_t aCount)
	{
		return Counter(aCount);
	}
	inline static Counter From(uint8_t aFlags, uint32_t aCount)
	{
		return Counter(aCount, aFlags);
	}
	inline static Counter From(uint8_t aFlags, uint32_t aCount, int64_t aTime)
	{
		return Counter(aCount, aFlags, aTime);
	}
};

struct FrozenCounterFactory: private openpal::PureStatic
{
	inline static FrozenCounter From(uint32_t aCount)
	{
		return FrozenCounter(aCount);
	}
	inline static FrozenCounter From(uint8_t aFlags, uint32_t aCount)
	{
		return FrozenCounter(aCount, aFlags);
	}
	inline static FrozenCounter From(uint8_t aFlags, uint32_t aCount, int64_t aTime)
	{
		return FrozenCounter(aCount, aFlags, aTime);
	}
};

struct ControlRelayOutputBlockFactory: private openpal::PureStatic
{
	inline static ControlRelayOutputBlock From(
	    uint8_t aCode,
	    uint8_t aCount,
	    uint32_t aOnTime,
	    uint32_t aOffTime,
	    CommandStatus aStatus)
	{
		ControlRelayOutputBlock crob(ControlCodeFromType(aCode), aCount, aOnTime, aOffTime, aStatus);
		crob.rawCode = aCode;
		return crob;
	}
};

struct BinaryOutputStatusFactory: private openpal::PureStatic
{
	inline static BinaryOutputStatus From(uint8_t flags)
	{
		return BinaryOutputStatus(flags);
	}

	inline static BinaryOutputStatus From(uint8_t flags, int64_t time)
	{
		return BinaryOutputStatus(flags, time);
	}
};


struct AnalogOutputStatusFactory: private openpal::PureStatic
{
	inline static AnalogOutputStatus From(uint8_t flags, double value)
	{
		return AnalogOutputStatus(value, flags);
	}

	inline static AnalogOutputStatus From(uint8_t flags, double value, int64_t time)
	{
		return AnalogOutputStatus(value, flags, time);
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
