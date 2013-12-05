#include "DNPCrc.h"

#include "CRC.h"

#include <openpal/Serialization.h>

namespace opendnp3
{

unsigned int DNPCrc::mpCrcTable[256];

//initialize the table
bool DNPCrc::mIsInitialized = DNPCrc::InitCrcTable();

unsigned int DNPCrc::CalcCrc(const uint8_t* aInput, size_t aLength)
{
	return CRC::CalcCRC(aInput, aLength, mpCrcTable, 0x0000, true);
}

void DNPCrc::AddCrc(uint8_t* aInput, size_t aLength)
{
	unsigned int crc = DNPCrc::CalcCrc(aInput, aLength);

	aInput[aLength] = crc & 0xFF; //set the LSB
	aInput[aLength + 1] = (crc >> 8) & 0xFF; //set the MSB
}

bool DNPCrc::IsCorrectCRC(const uint8_t* aInput, size_t aLength)
{
	return CalcCrc(aInput, aLength) == openpal::UInt16::Read(aInput + aLength);
}

bool DNPCrc::InitCrcTable()
{
	CRC::PrecomputeCRC(mpCrcTable, 0xA6BC);
	return true;
}

}

