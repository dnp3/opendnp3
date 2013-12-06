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
#ifndef __DATA_TYPES_H_
#define __DATA_TYPES_H_

#include "BaseDataTypes.h"
#include "MeasurementType.h"

namespace opendnp3
{

/**
	The Binary data type is for describing on-off (boolean) type values. Good examples of
	binaries are alarms, mode settings, enabled/disabled flags etc. Think of it as a status
	LED on a piece of equipment.
*/
class Binary : public BoolDataPoint
{
public:
	Binary(bool aValue, uint8_t aQuality = BQ_RESTART) : BoolDataPoint(BQ_RESTART, BQ_STATE) {
		SetQuality(aQuality);
		SetValue(aValue);
	}
	Binary() : BoolDataPoint(BQ_RESTART, BQ_STATE) {}

	typedef bool ValueType;
	typedef BinaryQuality QualityType;

	// Describes the static data type of the measurement as an enum
	static const MeasurementType MeasEnum = MeasurementType ::BINARY;

	static const int ONLINE = BQ_ONLINE;

	operator ValueType() const {
		return this->GetValue();
	}
	ValueType operator=(ValueType aValue) {
		this->SetValue(aValue);
		return GetValue();
	}
};

/**
	ControlStatus is used for describing the current state of a control. It is very infrequently
	used and many masters don't provide any mechanisms for reading these values so their use is
	strongly discouraged, a Binary should be used instead.
*/
class ControlStatus : public BoolDataPoint
{
public:

	ControlStatus(bool aValue, uint8_t aQuality = TQ_RESTART) : BoolDataPoint(TQ_RESTART, TQ_STATE) {
		SetValue(aValue);
		SetQuality(aQuality);
	}

	ControlStatus() : BoolDataPoint(TQ_RESTART, TQ_STATE) {}

	typedef bool ValueType;
	typedef ControlQuality QualityType;

	static const MeasurementType MeasEnum = MeasurementType::CONTROL_STATUS;

	static const int ONLINE = TQ_ONLINE;

	operator ValueType() const {
		return this->GetValue();
	}
	ValueType operator=(ValueType aValue) {
		this->SetValue(aValue);
		return GetValue();
	}
};

/**
	Analogs are used for variable data points that usuually reflect a real world value.
	Good examples are current, voltage, sensor readouts, etc. Think of a speedometer gauge.
*/

class Analog : public TypedDataPoint<double>
{
public:
	Analog() : TypedDataPoint<double>(AQ_RESTART) {}

	Analog(double aVal, uint8_t aQuality = AQ_RESTART) : TypedDataPoint<double>(AQ_RESTART) {
		SetValue(aVal);
		SetQuality(aQuality);
	}


	typedef double ValueType;
	typedef AnalogQuality QualityType;

	static const MeasurementType MeasEnum = MeasurementType::ANALOG;

	static const int ONLINE = AQ_ONLINE;

	operator ValueType() const {
		return this->GetValue();
	}
	ValueType operator=(ValueType aValue) {
		this->SetValue(aValue);
		return GetValue();
	}


};

/**
	Counters are used for describing generally increasing values (non-negative!). Good examples are
	total power consumed, max voltage. Think odometer on a car.
*/
class Counter : public TypedDataPoint<uint32_t>
{
public:
	Counter() : TypedDataPoint<uint32_t>(CQ_RESTART) {}
	Counter(uint32_t aVal, uint8_t aQuality = CQ_RESTART) : TypedDataPoint<uint32_t>(CQ_RESTART) {
		SetValue(aVal);
		SetQuality(aQuality);
	}

	typedef uint8_t ValueType;
	typedef CounterQuality QualityType;

	static const int ONLINE = CQ_ONLINE;

	static const MeasurementType MeasEnum = MeasurementType::COUNTER;

	operator ValueType() const {
		return this->GetValue();
	}
	ValueType operator=(ValueType aValue) {
		this->SetValue(aValue);
		return GetValue();
	}
};

/**
	Describes the last set value of the setpoint. Like the ControlStatus data type it is not
	well supportted and its generally better practice to use an explict analog.
*/
class SetpointStatus : public TypedDataPoint<double>
{
public:
	SetpointStatus() : TypedDataPoint<double>(PQ_RESTART) {}
	SetpointStatus(double aVal, uint8_t aQuality = PQ_RESTART) : TypedDataPoint<double>(PQ_RESTART) {
		SetValue(aVal);
		SetQuality(aQuality);
	}

	typedef double ValueType;
	typedef SetpointQuality QualityType;

	static const int ONLINE = PQ_ONLINE;

	static const MeasurementType MeasEnum = MeasurementType::SETPOINT_STATUS;

	operator ValueType() const {
		return this->GetValue();
	}
	ValueType operator=(ValueType aValue) {
		this->SetValue(aValue);
		return GetValue();
	}
};

}

#endif
