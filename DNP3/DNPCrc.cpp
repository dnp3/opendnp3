
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "DNPCrc.h"
#include <APL/CRC.h>

#include <APL/PackingUnpacking.h>

namespace apl
{
namespace dnp
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
	return CalcCrc(aInput, aLength) == UInt16LE::Read(aInput + aLength);
}

bool DNPCrc::InitCrcTable()
{
	CRC::PrecomputeCRC(mpCrcTable, 0xA6BC);
	return true;
}

}
}
