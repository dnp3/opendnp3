#include "CRC.h"

namespace opendnp3
{

unsigned int CRC::CalcCRC(const uint8_t* aInput, size_t aLength, const unsigned int* apTable, unsigned int aStart, bool aInvert)
{
	unsigned int CRC, index;

	CRC = aStart;

	for(size_t i = 0; i < aLength; i++) {
		index = (CRC ^ aInput[i]) & 0xFF;
		CRC = apTable[index] ^ (CRC >> 8);
	}

	if(aInvert) CRC = (~CRC) & 0xFFFF;

	return CRC;
}

void CRC::PrecomputeCRC(unsigned int* apTable, unsigned int aPolynomial)
{
	unsigned int i, j, CRC;

	for(i = 0; i < 256; i++) {
		CRC = i;
		for (j = 0; j < 8; ++j) {
			if(CRC & 0x0001) CRC = (CRC >> 1) ^ aPolynomial;
			else CRC >>= 1;
		}
		apTable[i] = CRC;
	}
}
}
