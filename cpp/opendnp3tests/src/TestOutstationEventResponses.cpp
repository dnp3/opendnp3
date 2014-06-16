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

#include "OutstationTestObject.h"

#include <opendnp3/DNPErrorCodes.h>


using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationEventResponsesTestSuite - " name

TEST_CASE(SUITE("BlankExceptionScan"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 01 3C 02 06"); // Read class 1
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReceiveNewRequestSolConfirmWait"))
{
	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::BinaryOnly(1), EventBufferConfig::AllTypes(10));
	t.LowerLayerUp();

	t.Transaction([](Database& db) { db.Update(Binary(true, BQ_ONLINE), 0); });

	t.SendToOutstation("C0 01 3C 02 06");
	REQUIRE(t.lower.PopWriteAsHex() == "E0 81 80 00 02 01 28 01 00 00 00 81");
	t.OnSendResult(true);

	t.SendToOutstation("C1 01 3C 02 06");
	REQUIRE(t.lower.PopWriteAsHex() == "E1 81 80 00 02 01 28 01 00 00 00 81");
}

TEST_CASE(SUITE("ReadClass1WithSOE"))
{
	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::AllTypes(100), EventBufferConfig::AllTypes(10));

	t.LowerLayerUp();

	t.Transaction([](Database& db) {
		db.Update(Analog(0x1234, AQ_ONLINE), 0x17); // 0x 12 34 00 00 in little endian
		db.Update(Binary(true, BQ_ONLINE), 0x10);
		db.Update(Analog(0x2222, AQ_ONLINE), 0x17); // 0x 22 22 00 00 in little endian
	});

	t.SendToOutstation("C0 01 3C 02 06");
	REQUIRE(t.lower.PopWriteAsHex() == "E0 81 80 00 20 01 28 01 00 17 00 01 34 12 00 00 02 01 28 01 00 10 00 81 20 01 28 01 00 17 00 01 22 22 00 00");
	t.OnSendResult(true);
	t.SendToOutstation("C0 00");

	t.SendToOutstation("C1 01 3C 02 06");		// Repeat read class 1
	REQUIRE(t.lower.PopWriteAsHex() == "C1 81 80 00");	// Buffer should have been cleared
}

TEST_CASE(SUITE("MultipleClasses"))
{
	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::AllTypes(1), EventBufferConfig::AllTypes(10));
	t.LowerLayerUp();

	t.Transaction([](Database& db) {
		db.staticData.binaries.metadata[0].clazz = PointClass::CLASS_1;
		db.staticData.analogs.metadata[0].clazz = PointClass::CLASS_2;
		db.staticData.counters.metadata[0].clazz = PointClass::CLASS_3;

		db.Update(Binary(true), 0);
		db.Update(Analog(3), 0);
		db.Update(Counter(7), 0);
	});	

	t.SendToOutstation("C0 01"); // empty READ
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 8E 00"); // all event bits set + restart
	t.OnSendResult(true);

	// ------ read 1 event at a time by class, until all events are gone ----

	t.SendToOutstation("C1 01 3C 03 06"); // Class 2
	REQUIRE(t.lower.PopWriteAsHex() == "E1 81 8A 00 20 01 28 01 00 00 00 01 03 00 00 00"); // restart + Class 1/3
	t.OnSendResult(true);
	t.SendToOutstation("C1 00");

	t.SendToOutstation("C2 01 3C 04 06"); // Class 3
	REQUIRE(t.lower.PopWriteAsHex() == "E2 81 82 00 16 01 28 01 00 00 00 01 07 00 00 00"); // restart + Class 1/3
	t.OnSendResult(true);
	t.SendToOutstation("C2 00");

	t.SendToOutstation("C3 01 3C 02 06"); // Class 1
	REQUIRE(t.lower.PopWriteAsHex() == "E3 81 80 00 02 01 28 01 00 00 00 81"); // restart only
	t.OnSendResult(true);
	t.SendToOutstation("C3 00");

	t.SendToOutstation("C4 01"); // empty READ
	REQUIRE(t.lower.PopWriteAsHex() == "C4 81 80 00"); // restart only
}

void TestEventRead(const std::string& request, const std::string& response, const std::function<void(Database& db)>& loadFun)
{

	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::AllTypes(1), EventBufferConfig::AllTypes(10));
	t.LowerLayerUp();

	t.Transaction([&](Database& db){ loadFun(db); });
	
	t.SendToOutstation(request);
	REQUIRE(t.lower.PopWriteAsHex() ==  response);
}

TEST_CASE(SUITE("ReadGrp2Var0"))
{
	auto update = [](Database& db)
	{
		db.Update(Binary(false, BQ_ONLINE), 0);
	};

	TestEventRead("C0 01 02 00 06", "E0 81 80 00 02 01 28 01 00 00 00 01", update);
}

TEST_CASE(SUITE("ReadGrp22Var0"))
{
	auto update = [](Database & db)
	{
		db.Update(Counter(0, CQ_ONLINE), 0);
	};

	TestEventRead("C0 01 16 00 06", "E0 81 80 00 16 01 28 01 00 00 00 01 00 00 00 00", update);
}

TEST_CASE(SUITE("ReadGrp32Var0"))
{
	auto update = [](Database & db)
	{
		db.Update(Analog(0.0, AQ_ONLINE), 0);
	};

	TestEventRead("C0 01 20 00 06", "E0 81 80 00 20 01 28 01 00 00 00 01 00 00 00 00", update);
}

/*
TEST_CASE(SUITE("ReadGrp2Var1"))
{
	auto update = [](Database & db)
	{
		db.Update(Binary(false, BQ_ONLINE), 0);
	};

	TestEventRead("C0 01 02 01 06", "E0 81 80 00 02 01 17 01 00 01", update); // 1 byte count == 1, ONLINE quality
}


TEST_CASE(SUITE("ReadGrp2Var2"))
{
	TestEventRead("C0 01 02 02 06", "E0 81 80 00 02 02 17 01 00 01 00 00 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp2Var3"))
{
	TestEventRead("C0 01 02 03 06", "E0 81 80 00 33 01 07 01 00 00 00 00 00 00 02 03 17 01 00 01 00 00"); // 1 byte count == 1, ONLINE quality
}
*/


/*
TEST_CASE(SUITE("ComplexReadSequence"))
{

const size_t NUM = 4;
OutstationConfig config; cfg.disableUnsol = true;
OutstationTestObject t(cfg);
t.db.Configure(MeasurementType::BINARY, NUM);
t.db.SetClass(MeasurementType::BINARY, CLASS_1);
t.outstation.OnLowerLayerUp();

{
Transaction tr(&t.db);
for(size_t i = 0; i < NUM; ++i) t.db.Update(Binary(false, BQ_ONLINE), i);
}

//request
std::string request("C0 01");
std::string grp2Var2x2("02 02 07 02");
std::string grp2Var0("02 00 06");


//response
std::string rsp("E0 81 80 00");
std::string grp2Var2hdr("02 02 17 02");
std::string grp2Var1hdr("02 01 17 02");
std::string grp2Var2rsp("01 00 00 00 00 00 00"); //minus the index


request.append(" ").append(grp2Var2x2).append(" ").append(grp2Var0);
rsp.append(" ").append(grp2Var2hdr).append(" 00 ").append(grp2Var2rsp).append(" 01 ").append(grp2Var2rsp);
rsp.append(" ").append(grp2Var1hdr).append(" 02 01 03 01");


t.SendToOutstation(request);
REQUIRE(t.lower.PopWriteAsHex() ==  rsp);
}
*/



