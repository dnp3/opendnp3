#ifndef __OPENPAL_SERIALIZATION_H_
#define __OPENPAL_SERIALIZATION_H_

#include <openpal/ReverseEndian.h>

namespace openpal
{

/*
Users should only use these typedefs. This will allow these to be switched
if we ever need to support systems with reverse endian
*/

typedef UInt8Simple			UInt8;
typedef Bit16LE<int16_t>	Int16;
typedef Bit16LE<uint16_t>	UInt16;
typedef Bit32LE<int32_t>	Int32;
typedef Bit32LE<uint32_t>	UInt32;
typedef UInt48LE			UInt48;
typedef SingleFloatSafe		SingleFloat;
typedef DoubleFloatSafe		DoubleFloat;

}

#endif
