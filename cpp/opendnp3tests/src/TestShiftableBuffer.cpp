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
#include <catch.hpp>

#include "Exception.h"

#include <opendnp3/link/ShiftableBuffer.h>

#include <openpal/StaticBuffer.h>

#include <cstring>

using namespace opendnp3;


#define SUITE(name) "ShiftableBufferSuite - " name

const static uint8_t SYNC[] = {0x05, 0x64};

openpal::StaticBuffer<100> staticBuffer;

TEST_CASE(SUITE("ConstructDestruct"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());
}

TEST_CASE(SUITE("InitialState"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	REQUIRE(b.NumReadBytes() ==  0);
	REQUIRE(b.NumWriteBytes() ==  100);
	REQUIRE(b.ReadBuff() ==  b.WriteBuff());
}

TEST_CASE(SUITE("ReadingWriting"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	b.AdvanceWrite(40);
	REQUIRE(b.NumWriteBytes() ==  60);
	REQUIRE(b.NumReadBytes() ==  40);

	b.AdvanceWrite(60);
	REQUIRE(b.NumWriteBytes() ==  0);
	REQUIRE(b.NumReadBytes() ==  100);

	b.AdvanceRead(30);
	REQUIRE(b.NumWriteBytes() ==  0);
	REQUIRE(b.NumReadBytes() ==  70);

	b.AdvanceRead(70);
	REQUIRE(b.NumWriteBytes() ==  0);
	REQUIRE(b.NumReadBytes() ==  0);
}

TEST_CASE(SUITE("Shifting"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;
	b.WriteBuff()[97] = 1;
	b.WriteBuff()[98] = 2;
	b.WriteBuff()[99] = 3;

	b.AdvanceWrite(100);

	b.AdvanceRead(97);
	b.Shift();

	REQUIRE(b[0] ==  1);
	REQUIRE(b[1] ==  2);
	REQUIRE(b[2] ==  3);
}

TEST_CASE(SUITE("SyncNoPattern"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	for (size_t i = 0; i < b.NumWriteBytes(); ++i)
	{
		b.WriteBuff()[i] = 0;
	}

	b.AdvanceWrite(100);

	REQUIRE_FALSE(b.Sync());
	REQUIRE(b.NumReadBytes() ==  1); // 1 byte left since need 2 bytes to sync
	REQUIRE(b.NumWriteBytes() ==  0);
}

TEST_CASE(SUITE("SyncBeginning"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;

	memcpy(b.WriteBuff(), SYNC, 2);
	b.AdvanceWrite(100);

	REQUIRE(b.Sync());
	REQUIRE(b.NumReadBytes() ==  100);
	REQUIRE(b.NumWriteBytes() ==  0);

}

TEST_CASE(SUITE("SyncFullPattern"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;
	uint8_t pattern[] = {0x05, 0x64};
	memcpy(b.WriteBuff() + 50, pattern, 2); //copy the pattern into the buffer
	b.AdvanceWrite(100);

	REQUIRE(b.Sync());
	REQUIRE(b.NumReadBytes() ==  50);
	REQUIRE(b.NumWriteBytes() ==  0);

	// Check that the sync operation correctly advanced the reader
	REQUIRE(b[0] ==  SYNC[0]);
	REQUIRE(b[1] ==  SYNC[1]);
}

TEST_CASE(SUITE("SyncPartialPattern"))
{
	ShiftableBuffer b(staticBuffer.Buffer(), staticBuffer.Size());

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;

	b.WriteBuff()[97] = 0x05;
	b.AdvanceWrite(98);

	REQUIRE_FALSE(b.Sync());
	REQUIRE(b.NumReadBytes() ==  1);
	REQUIRE(b.NumWriteBytes() ==  2);

	// Check that the sync operation correctly advanced the reader
	REQUIRE(b[0] ==  SYNC[0]);
}


