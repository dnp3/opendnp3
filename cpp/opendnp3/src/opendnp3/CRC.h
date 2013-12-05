#ifndef __CRC_H_
#define __CRC_H_

#include <openpal/Visibility.h>

#include <stdint.h>
#include <stddef.h>

namespace opendnp3
{

class DLL_LOCAL CRC
{
public:
	static unsigned int CalcCRC(const uint8_t* aInput, size_t aLength, const unsigned int* apTable, unsigned int aStart, bool aInvert);
	static void PrecomputeCRC(unsigned int* apTable, unsigned int aPolynomial);
};

}

#endif
