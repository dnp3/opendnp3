/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "DNPCrc.h"

#include <openpal/Serialization.h>

namespace opendnp3
{

uint16_t DNPCrc::crcTable[256];

// initialize the static table
bool DNPCrc::isInitialized = DNPCrc::InitCrcTable();

uint16_t DNPCrc::CalcCrc(const uint8_t* input, uint32_t length)
{
	uint8_t index;

	uint16_t CRC = 0x0000;

	for (uint32_t i = 0; i < length; ++i)
	{
		index = (CRC ^ input[i]) & 0xFF;
		CRC = crcTable[index] ^ (CRC >> 8);
	}	

	return ((~CRC) & 0xFFFF);
}

void DNPCrc::AddCrc(uint8_t* input, uint32_t length)
{
	unsigned int crc = DNPCrc::CalcCrc(input, length);

	input[length] = crc & 0xFF; //set the LSB
	input[length + 1] = (crc >> 8) & 0xFF; //set the MSB
}

bool DNPCrc::IsCorrectCRC(const uint8_t* input, uint32_t length)
{
	return CalcCrc(input, length) == openpal::UInt16::Read(input + length);
}

bool DNPCrc::InitCrcTable()
{	
	uint16_t i, j, CRC;

	for (i = 0; i < 256; i++)
	{
		CRC = i;
		for (j = 0; j < 8; ++j)
		{
			if (CRC & 0x0001) CRC = (CRC >> 1) ^ 0xA6BC;
			else CRC >>= 1;
		}
		crcTable[i] = CRC;
	}
	return true;
}

}

