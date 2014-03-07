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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "Exception.h"

#include <opendnp3/link/ShiftableBuffer.h>

using namespace opendnp3;


BOOST_AUTO_TEST_SUITE(ShiftableBufferSuite)

const static uint8_t SYNC[] = {0x05, 0x64};

BOOST_AUTO_TEST_CASE(ConstructDestruct)
{
	ShiftableBuffer b(100);
}

BOOST_AUTO_TEST_CASE(InitialState)
{
	ShiftableBuffer b(100);

	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 0);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 100);
	BOOST_REQUIRE_EQUAL(b.ReadBuff(), b.WriteBuff());

	BOOST_REQUIRE_THROW(b.AdvanceWrite(101), ArgumentException);
	BOOST_REQUIRE_THROW(b.AdvanceRead(1), ArgumentException);
}

BOOST_AUTO_TEST_CASE(ReadingWriting)
{
	ShiftableBuffer b(100);

	b.AdvanceWrite(40);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 60);
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 40);

	b.AdvanceWrite(60);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 100);

	b.AdvanceRead(30);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 70);

	b.AdvanceRead(70);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 0);
}

BOOST_AUTO_TEST_CASE(Shifting)
{
	ShiftableBuffer b(100);

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;
	b.WriteBuff()[97] = 1;
	b.WriteBuff()[98] = 2;
	b.WriteBuff()[99] = 3;

	b.AdvanceWrite(100);

	b.AdvanceRead(97);
	b.Shift();

	BOOST_REQUIRE_EQUAL(b[0], 1);
	BOOST_REQUIRE_EQUAL(b[1], 2);
	BOOST_REQUIRE_EQUAL(b[2], 3);
}

BOOST_AUTO_TEST_CASE(SyncNoPattern)
{
	ShiftableBuffer b(100);
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;

	b.AdvanceWrite(100);

	BOOST_REQUIRE_FALSE(b.Sync(SYNC, 2));
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 0);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);
}

BOOST_AUTO_TEST_CASE(SyncBeginning)
{
	ShiftableBuffer b(100);
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;

	memcpy(b.WriteBuff(), SYNC, 2);
	b.AdvanceWrite(100);

	BOOST_REQUIRE(b.Sync(SYNC, 2));
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 100);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);

}

BOOST_AUTO_TEST_CASE(SyncFullPattern)
{
	ShiftableBuffer b(100);

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;
	uint8_t pattern[] = {0x05, 0x64};
	memcpy(b.WriteBuff() + 50, pattern, 2); //copy the pattern into the buffer
	b.AdvanceWrite(100);

	BOOST_REQUIRE(b.Sync(SYNC, 2));
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 50);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 0);

	// Check that the sync operation correctly advanced the reader
	BOOST_REQUIRE_EQUAL(b[0], SYNC[0]);
	BOOST_REQUIRE_EQUAL(b[1], SYNC[1]);
}

BOOST_AUTO_TEST_CASE(SyncPartialPattern)
{
	ShiftableBuffer b(100);

	//initialize buffer to all zeros
	for(size_t i = 0; i < b.NumWriteBytes(); ++i) b.WriteBuff()[i] = 0;

	b.WriteBuff()[97] = 0x05;
	b.AdvanceWrite(98);

	BOOST_REQUIRE_FALSE(b.Sync(SYNC, 2));
	BOOST_REQUIRE_EQUAL(b.NumReadBytes(), 1);
	BOOST_REQUIRE_EQUAL(b.NumWriteBytes(), 2);

	// Check that the sync operation correctly advanced the reader
	BOOST_REQUIRE_EQUAL(b[0], SYNC[0]);
}

BOOST_AUTO_TEST_SUITE_END()
