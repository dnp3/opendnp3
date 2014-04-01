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


#include "MockLogSubscriber.h"
#include "OutstationTestObject.h"

#include <opendnp3/DNPErrorCodes.h>


using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationEventResponsesTestSuite - " name

TEST_CASE(SUITE("BlankExceptionScan"))
{
	OutstationConfig cfg; cfg.mDisableUnsol = true;
	OutstationTestObject t(cfg, DatabaseTemplate());
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 01 3C 02 06"); // Read class 1
	REQUIRE(t.Read() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReadClass1WithSOE"))
{
	OutstationConfig cfg; cfg.mDisableUnsol = true;
	OutstationTestObject t(cfg, DatabaseTemplate::AllTypes(100));
		
	t.db.staticData.binaries.metadata[0x10].clazz = CLASS_1;
	t.db.staticData.analogs.metadata[0x17].clazz = CLASS_1;

	t.outstation.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		
		t.db.Update(Analog(0x1234, AQ_ONLINE), 0x17); // 0x 12 34 00 00 in little endian
		t.db.Update(Binary(true, BQ_ONLINE), 0x10);			
		t.db.Update(Analog(0x2222, AQ_ONLINE), 0x17); // 0x 22 22 00 00 in little endian		
	}

	t.SendToOutstation("C0 01 3C 02 06");
		
	REQUIRE(t.Read() == "E0 81 80 00 20 01 28 01 00 17 00 01 34 12 00 00 02 01 28 01 00 10 00 81 20 01 28 01 00 17 00 01 22 22 00 00");

	t.SendToOutstation("C0 01 3C 02 06");		// Repeat read class 1
	REQUIRE(t.Read() ==  "C0 81 80 00");	// Buffer should have been cleared
}

TEST_CASE(SUITE("MultipleClasses"))
{
	OutstationConfig cfg; cfg.mDisableUnsol = true;
	OutstationTestObject t(cfg, DatabaseTemplate::AllTypes(1));
	t.outstation.OnLowerLayerUp();

	t.db.staticData.binaries.metadata[0].clazz = PointClass::CLASS_1;
	t.db.staticData.analogs.metadata[0].clazz = PointClass::CLASS_2;
	t.db.staticData.counters.metadata[0].clazz = PointClass::CLASS_3;

	{
		Transaction tr(&t.db);
		t.db.Update(Binary(true), 0);
		t.db.Update(Analog(3), 0);
		t.db.Update(Counter(7), 0);
	}

	t.SendToOutstation("C0 01"); // empty READ
	REQUIRE(t.Read() == "C0 81 8E 00"); // all event bits set + restart

	// ------ read 1 event at a time by class, until all events are gone ----
	
	t.SendToOutstation("C0 01 3C 03 06"); // Class 2
	REQUIRE(t.Read() == "E0 81 8A 00 20 01 28 01 00 00 00 01 03 00 00 00"); // restart + Class 1/3

	t.SendToOutstation("C0 01 3C 04 06"); // Class 3
	REQUIRE(t.Read() == "E0 81 82 00 16 01 28 01 00 00 00 01 07 00 00 00"); // restart + Class 1/3

	t.SendToOutstation("C0 01 3C 02 06"); // Class 1
	REQUIRE(t.Read() == "E0 81 80 00 02 01 28 01 00 00 00 81"); // restart only

	t.SendToOutstation("C0 01"); // empty READ
	REQUIRE(t.Read() == "C0 81 80 00"); // restart only
}

void TestEventRead(const std::function<void(Database& db)>& loadFun, const std::string& request, const std::string& response)
{

	OutstationConfig cfg; cfg.mDisableUnsol = true;
	OutstationTestObject t(cfg, DatabaseTemplate::AllTypes(1), PointClass::CLASS_1);
	t.outstation.OnLowerLayerUp();	

	{
		Transaction tr(&t.db);
		loadFun(t.db);		
	}

	t.SendToOutstation(request);
	REQUIRE(t.Read() ==  response);
}


TEST_CASE(SUITE("ReadGrp2Var0"))
{	
	TestEventRead(
		[](Database& db) { db.Update(Binary(false, BQ_ONLINE), 0); },
		"C0 01 02 00 06", "E0 81 80 00 02 01 28 01 00 00 00 01"
	);
}

TEST_CASE(SUITE("ReadGrp22Var0"))
{
	TestEventRead(
		[](Database& db) { db.Update(Counter(0, CQ_ONLINE), 0); },
		"C0 01 16 00 06", "E0 81 80 00 16 01 28 01 00 00 00 01 00 00 00 00"
	);
}

TEST_CASE(SUITE("ReadGrp32Var0"))
{
	TestEventRead(
		[](Database& db) { db.Update(Analog(0.0, AQ_ONLINE), 0); },
		"C0 01 20 00 06", "E0 81 80 00 20 01 28 01 00 00 00 01 00 00 00 00"
	);
}

/*
TEST_CASE(SUITE("ReadGrp2Var1"))
{
TestEventRead("C0 01 02 01 06", "E0 81 80 00 02 01 17 01 00 01"); // 1 byte count == 1, ONLINE quality
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
OutstationConfig config; cfg.mDisableUnsol = true;
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
REQUIRE(t.Read() ==  rsp);
}
*/

/*
TEST_CASE(SUITE("NullUnsolOnStartup"))
{
	OutstationConfig cfg;  cfg.mAllowTimeSync = true;
	OutstationTestObject t(cfg);
	t.outstation.OnLowerLayerUp();

	// Null UNSOL, FIR, FIN, CON, UNS, w/ restart and need-time IIN
	REQUIRE(t.Read() ==  "F0 82 90 00");
}

TEST_CASE(SUITE("UnsolRetryDelay"))
{
	OutstationConfig cfg;
	OutstationTestObject t(cfg);
	t.app.EnableAutoSendCallback(false); // will respond with failure
	t.outstation.OnLowerLayerUp();

	// check for the startup null unsol packet, but fail the transaction
	REQUIRE(t.Read() ==  "F0 82 80 00");
	REQUIRE(t.mts.NumActive() ==  1); // this should cause a timer to become active
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00");
}

TEST_CASE(SUITE("UnsolData"))
{
	OutstationConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	// do a transaction before the layer comes online to prove that the null transaction
	// is occuring before unsol data is sent
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	t.outstation.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	// should immediately try to send another unsol packet,
	// Grp2Var1, qual 0x17, count 1, index 0, quality+val == 0x01
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 01");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent
}


TEST_CASE(SUITE("UnsolicitedStaysDisabledEvenIfDataAreLoadedPriorToOpen"))
{
OutstationConfig cfg; cfg.mDisableUnsol = true;
OutstationTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

{
Transaction tx(&t.db);
t.db.Update(Analog(0), 0);
}

t.outstation.OnLowerLayerUp();

// Outstation shouldn't send an unsolicited handshake b/c unsol it disabled
REQUIRE(t.NothingToRead());
}

TEST_CASE(SUITE("UnsolicitedStaysDisabledEvenIfDataAreLoadedAfterOpen"))
{
OutstationConfig cfg; cfg.mDisableUnsol = true;
OutstationTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

auto pObs = t.outstation.GetDataObserver();

t.outstation.OnLowerLayerUp();

{
Transaction t(pObs);
pObs->Update(Analog(0), 0);
}

REQUIRE(t.mts.Dispatch() > 0);

// Outstation shouldn't send an unsolicited handshake b/c unsol it disabled
REQUIRE(t.NothingToRead());
}

TEST_CASE(SUITE("UnsolEventBufferOverflow"))
{
	OutstationConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	cfg.mEventMaxConfig.mMaxBinaryEvents = 2; // set the max to 2 to make testing easy
	cfg.mUnsolPackDelay = TimeDuration::Milliseconds(0);
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	// null unsol
	t.outstation.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	// this transaction will overflow the event buffer
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	// should immediately try to send 2 unsol events
	// Grp2Var1, qual 0x17, count 2, index 0
	// The last two values should be published, 0x01 and 0x81 (false and true)
	// the first value is lost off the front of the buffer
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 02 00 01 00 81");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent
}

TEST_CASE(SUITE("UnsolMultiFragments"))
{
	OutstationConfig cfg;
	cfg.mMaxFragSize = 10; //this will cause the unsol response to get fragmented
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 2);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	t.outstation.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 1);
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	REQUIRE(t.mts.NumActive() ==  1); // unsol pack timer should be active

	REQUIRE(t.mts.DispatchOne()); //dispatch the unsol pack timer

	// Only enough room to in the APDU to carry a single value
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 01 01");
	// should immediately try to send another unsol packet
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 01");
}

// Test that non-read fragments are immediately responded to while waiting for a
// response to unsolicited data
TEST_CASE(SUITE("WriteDuringUnsol"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	//now send a write IIN request, and test that the outstation answers immediately
	t.SendToOutstation("C0 02 50 01 00 07 07 00");
	REQUIRE(t.Read() ==  "C0 81 00 00");

	t.outstation.OnUnsolSendSuccess();
	REQUIRE(t.Count() ==  0);
}

TEST_CASE(SUITE("ReadDuringUnsol"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToOutstation("C0 01 3C 02 06");

	t.outstation.OnUnsolSendSuccess();
	REQUIRE(t.Read() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReadWriteDuringUnsol"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToOutstation("C0 01 3C 01 06");

	//now send a write IIN request, and test that the outstation answers immediately
	t.SendToOutstation("C0 02 50 01 00 07 07 00");
	t.outstation.OnUnsolSendSuccess();
	REQUIRE(t.Read() ==  "C0 81 00 00");
}

TEST_CASE(SUITE("UnsolEnable"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToOutstation("C0 14 3C 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 00");

	// should automatically send the previous data as unsol
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 01");
}

TEST_CASE(SUITE("UnsolEnableBadObject"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToOutstation("C0 14 01 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 01");
}

TEST_CASE(SUITE("UnsolEnableDisableFailure"))
{
	OutstationConfig cfg; cfg.mDisableUnsol = true;
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 14 3C 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 01"); //FUNC_NOT_SUPPORTED
}
*/



