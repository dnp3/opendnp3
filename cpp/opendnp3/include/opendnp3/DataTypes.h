#ifndef __DATA_TYPES_H_
#define __DATA_TYPES_H_

#include "BaseDataTypes.h"

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
	Binary(bool aValue, uint8_t aQuality = BQ_RESTART) : BoolDataPoint(BQ_RESTART, DT_BINARY, BQ_STATE) {
		SetQuality(aQuality);
		SetValue(aValue);
	}
	Binary() : BoolDataPoint(BQ_RESTART, DT_BINARY, BQ_STATE) {}

	typedef bool ValueType;
	typedef BinaryQuality QualityType;

	// Describes the static data type of the measurement as an enum
	static const DataTypes MeasEnum = DT_BINARY;

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

	ControlStatus(bool aValue, uint8_t aQuality = TQ_RESTART) : BoolDataPoint(TQ_RESTART, DT_CONTROL_STATUS, TQ_STATE) {
		SetValue(aValue);
		SetQuality(aQuality);
	}

	ControlStatus() : BoolDataPoint(TQ_RESTART, DT_CONTROL_STATUS, TQ_STATE) {}

	typedef bool ValueType;
	typedef ControlQuality QualityType;

	static const DataTypes MeasEnum = DT_CONTROL_STATUS;

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
	Analog() : TypedDataPoint<double>(AQ_RESTART, DT_ANALOG) {}

	Analog(double aVal, uint8_t aQuality = AQ_RESTART) : TypedDataPoint<double>(AQ_RESTART, DT_ANALOG) {
		SetValue(aVal);
		SetQuality(aQuality);
	}


	typedef double ValueType;
	typedef AnalogQuality QualityType;

	static const DataTypes MeasEnum = DT_ANALOG;

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
	Counter() : TypedDataPoint<uint32_t>(CQ_RESTART, DT_COUNTER) {}
	Counter(uint32_t aVal, uint8_t aQuality = CQ_RESTART) : TypedDataPoint<uint32_t>(CQ_RESTART, DT_COUNTER) {
		SetValue(aVal);
		SetQuality(aQuality);
	}

	typedef uint8_t ValueType;
	typedef CounterQuality QualityType;

	static const int ONLINE = CQ_ONLINE;

	static const DataTypes MeasEnum = DT_COUNTER;

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
	SetpointStatus() : TypedDataPoint<double>(PQ_RESTART, DT_SETPOINT_STATUS) {}
	SetpointStatus(double aVal, uint8_t aQuality = PQ_RESTART) : TypedDataPoint<double>(PQ_RESTART, DT_SETPOINT_STATUS) {
		SetValue(aVal);
		SetQuality(aQuality);
	}

	typedef double ValueType;
	typedef SetpointQuality QualityType;

	static const int ONLINE = PQ_ONLINE;

	static const DataTypes MeasEnum = DT_SETPOINT_STATUS;

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
