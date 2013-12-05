#include "BufferTypes.h"

#include <openpal/Exception.h>
#include <openpal/Location.h>

namespace opendnp3
{

BufferTypes Convert(DataTypes aType)
{
	switch(aType) {
	case(DT_BINARY):	return BT_BINARY;
	case(DT_ANALOG):	return BT_ANALOG;
	case(DT_COUNTER):	return BT_COUNTER;
	default:
		MACRO_THROW_EXCEPTION_COMPLEX(openpal::ArgumentException, "Invalid conversion to BufferType: " << aType);
	}
}

}

/* vim: set ts=4 sw=4: */
