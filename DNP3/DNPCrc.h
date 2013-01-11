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
#ifndef __DNP_CRC_H_
#define __DNP_CRC_H_

#include <APL/Types.h>
#include <stddef.h>

namespace apl
{
namespace dnp
{

class DNPCrc
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
}

#endif
