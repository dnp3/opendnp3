/*
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

#ifndef OPENDNP3_MEASUREMENTCONFIG_H
#define OPENDNP3_MEASUREMENTCONFIG_H

#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/gen/PointClass.h"

namespace opendnp3
{

// All entries have this information
template <class Spec>
struct Entry
{
	// virtual index for discontiguous data, as opposed to the raw array index
	uint16_t vIndex = 0; 
	typename Spec::StaticVariation svariation = Spec::DefaultStaticVariation;
};

template <class Spec>
struct Deadband
{
	typename Spec::ValueType deadband = 0;
};

template <class Spec>
struct Event
{
	PointClass clazz;
	typename Spec::EventVariation evariation = Spec::DefaultEventVariation;
};

class BinaryConfig : public Entry<BinarySpec>, public Event<BinarySpec> {};
class DoubleBinaryConfig : public Entry<DoubleBitBinarySpec>, public Event<DoubleBitBinarySpec> {};
class AnalogConfig : public Entry<AnalogSpec>, public Event<AnalogSpec> {};
class CounterConfig : public Entry<CounterSpec>, public Event<CounterSpec>, public Deadband<CounterSpec> {};
class FrozenCounterConfig : public Entry<FrozenCounterSpec>, public Event<FrozenCounterSpec>, public Deadband<FrozenCounterSpec> {};
class BOStatusConfig : public Entry<BinaryOutputStatusSpec>, public Event<BinaryOutputStatusSpec> {};
class AOStatusConfig : public Entry<AnalogOutputStatusSpec>, public Event<AnalogOutputStatusSpec> {};
class TimeAndIntervalConfig : public Entry<TimeAndIntervalSpec> {};

}

#endif
