#include <opendnp3/PointClass.h>

namespace opendnp3
{

PointClass IntToPointClass(int aClass)
{
	switch(aClass) {
	case(PC_CLASS_1):
		return PC_CLASS_1;
	case(PC_CLASS_2):
		return PC_CLASS_2;
	case(PC_CLASS_3):
		return PC_CLASS_3;
	default:
		return PC_CLASS_0;
	}
}

}

