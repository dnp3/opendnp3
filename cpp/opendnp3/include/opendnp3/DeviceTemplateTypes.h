#ifndef __DEVICE_TEMPLATE_TYPES_H_
#define __DEVICE_TEMPLATE_TYPES_H_

#include <vector>

#include "PointClass.h"

namespace opendnp3
{

struct PointRecord {
	PointRecord() {}
};

/// Configuration type that has a PointClass member
struct EventPointRecord : public PointRecord {
	EventPointRecord(PointClass aPointClass) :
		PointRecord(),
		EventClass(aPointClass)
	{}

	EventPointRecord() : EventClass(PC_CLASS_1) {}

	/// when the point changes, it will generate an event unless EventClass == PC_CLASS_0
	PointClass EventClass;
};

/// DeadbandPointRecords have a parameter for controlling eventing tolerances
struct DeadbandPointRecord : public EventPointRecord {

	DeadbandPointRecord(PointClass aPointClass, double aDeadband) :
		EventPointRecord(aPointClass),
		Deadband(aDeadband)
	{}

	DeadbandPointRecord() : Deadband(0) {}

	/// Points can change value within the deadband and not trigger events
	double Deadband;
};

}

#endif
