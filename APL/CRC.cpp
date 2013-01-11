//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "CRC.h"

namespace apl
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
