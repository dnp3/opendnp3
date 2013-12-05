#ifndef __DNP_CRC_H_
#define __DNP_CRC_H_

#include <openpal/Visibility.h>

#include <stddef.h>
#include <cstdint>

namespace opendnp3
{

class DLL_LOCAL DNPCrc
{
public:

	static unsigned int CalcCrc(const uint8_t* aInput, size_t length);

	static void AddCrc(uint8_t* aInput, size_t aLength);

	static bool IsCorrectCRC(const uint8_t* aInput, size_t aLength);

private:

	static bool mIsInitialized;

	static bool InitCrcTable();

	static unsigned int mpCrcTable[256]; //Precomputed CRC lookup table

};

}

#endif
