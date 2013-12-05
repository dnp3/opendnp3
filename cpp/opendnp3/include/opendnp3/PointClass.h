#ifndef __POINT_CLASS_H_
#define __POINT_CLASS_H_

namespace opendnp3
{

/// Bitmasks for Class1/2/3 events
enum PointClass {
	PC_CLASS_0 = 0x01,
	PC_CLASS_1 = 0x02,
	PC_CLASS_2 = 0x04,
	PC_CLASS_3 = 0x08,
	PC_ALL_EVENTS = PC_CLASS_1 | PC_CLASS_2 | PC_CLASS_3,
	PC_INVALID = 0x10
};

///converts an integer to a PointClass enumeration
PointClass IntToPointClass(int aClass);

}

#endif

