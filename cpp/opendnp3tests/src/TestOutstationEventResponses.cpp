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
#include "SlaveTestObject.h"

#include <opendnp3/DNPConstants.h>


using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationEventResponsesTestSuite - " name

/*
TEST_CASE(SUITE("BlankExceptionScan"))
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 3C 02 06"); // Read class 1
	REQUIRE(t.Read() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReadClass1"))
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(100));

	t.db.staticData.analogs.metadata[0x10].clazz = CLASS_1;
	t.db.staticData.analogs.metadata[0x17].clazz = CLASS_1;
	t.db.staticData.analogs.metadata[0x05].clazz = CLASS_1;

	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(Analog(0x0987, AQ_ONLINE), 0x10); // 0x87 09 00 00 in little endian
		t.db.Update(Analog(0x1234, AQ_ONLINE), 0x17); // 0x39 30 00 00 in little endian
		t.db.Update(Analog(0x2222, AQ_ONLINE), 0x05); // 0x22 22 00 00 in little endian
		t.db.Update(Analog(0x3333, AQ_ONLINE), 0x05); // 0x33 33 00 00 in little endian
		t.db.Update(Analog(0x4444, AQ_ONLINE), 0x05); // 0x44 44 00 00 in little endian
	}

	t.SendToSlave("C0 01 3C 02 06");

	// The indices should be in reverse-order from how they were
	// added, but the values for a given index should be in the same order
	REQUIRE(t.Read() ==  "E0 81 80 00 20 01 17 05 10 01 87 09 00 00 17 01 34 12 00 00 05 01 22 22 00 00 05 01 33 33 00 00 05 01 44 44 00 00");

	t.SendToSlave("C0 01 3C 02 06");			// Repeat read class 1
	REQUIRE(t.Read() ==  "C0 81 80 00");	// Buffer should have been cleared
}

TEST_CASE(SUITE("ReadClass1TimeOrdered"))
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);

	t.db.Configure(MeasurementType::ANALOG, 100);
	t.db.mAnalogs[0x10].clazz = CLASS_1;
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);

		Analog a0(0x2222, AQ_ONLINE);
		a0.SetTime(10);

		Analog a1(0x4444, AQ_ONLINE);
		a1.SetTime(20);

		Analog a2(0x1111, AQ_ONLINE);
		a2.SetTime(5);

		Analog a3(0x3333, AQ_ONLINE);
		a3.SetTime(15);


		// Expected order in packet should be:
		// a2 -> a0 -> a3 -> a1
		t.db.Update(a0, 0x10);
		t.db.Update(a1, 0x10);
		t.db.Update(a2, 0x10);
		t.db.Update(a3, 0x10);
	}

	t.SendToSlave("C0 01 3C 02 06");

	// The indices should be in reverse-order from how they were
	// added, but the values for a given index should be in the same
	// order.
	REQUIRE(t.Read() ==  "E0 81 80 00 20 01 17 04 10 01 11 11 00 00 10 01 22 22 00 00 10 01 33 33 00 00 10 01 44 44 00 00");

	t.SendToSlave("C0 01 3C 02 06");			// Repeat read class 1
	REQUIRE(t.Read() ==  "C0 81 80 00");	// Buffer should have been cleared
}


// test that asking for a specific data type returns the requested type
void TestEventRead(const std::string& arRequest, const std::string& arResponse)
{
SlaveConfig cfg; cfg.mDisableUnsol = true;
SlaveTestObject t(cfg);
t.db.Configure(MeasurementType::BINARY, 1);
t.db.Configure(MeasurementType::ANALOG, 1);
t.db.Configure(MeasurementType::COUNTER, 1);
t.db.Configure(MeasurementType::CONTROL_STATUS, 1);
t.db.Configure(MeasurementType::SETPOINT_STATUS, 1);
t.db.SetClass(MeasurementType::BINARY, CLASS_1);
t.db.SetClass(MeasurementType::ANALOG, CLASS_1);
t.db.SetClass(MeasurementType::COUNTER, CLASS_1);
t.slave.OnLowerLayerUp();


{
Transaction tr(&t.db);
t.db.Update(Binary(false, BQ_ONLINE), 0);
t.db.Update(Counter(0, CQ_ONLINE), 0);
t.db.Update(Analog(0.0, AQ_ONLINE), 0);
t.db.Update(BinaryOutputStatus(false, TQ_ONLINE), 0);
t.db.Update(AnalogOutputStatus(0.0, PQ_ONLINE), 0);
}

t.SendToSlave(arRequest);
REQUIRE(t.Read() ==  arResponse);
}

TEST_CASE(SUITE("ReadGrp2Var0"))
{
TestEventRead("C0 01 02 00 06", "E0 81 80 00 02 01 17 01 00 01"); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp22Var0"))
{
TestEventRead("C0 01 16 00 06", "E0 81 80 00 16 01 17 01 00 01 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

TEST_CASE(SUITE("ReadGrp32Var0"))
{
TestEventRead("C0 01 20 00 06", "E0 81 80 00 20 01 17 01 00 01 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

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

TEST_CASE(SUITE("ComplexReadSequence"))
{
const size_t NUM = 4;
SlaveConfig cfg; cfg.mDisableUnsol = true;
SlaveTestObject t(cfg);
t.db.Configure(MeasurementType::BINARY, NUM);
t.db.SetClass(MeasurementType::BINARY, CLASS_1);
t.slave.OnLowerLayerUp();

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


t.SendToSlave(request);
REQUIRE(t.Read() ==  rsp);
}

TEST_CASE(SUITE("NullUnsolOnStartup"))
{
	SlaveConfig cfg;  cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Null UNSOL, FIR, FIN, CON, UNS, w/ restart and need-time IIN
	REQUIRE(t.Read() ==  "F0 82 90 00");
}

TEST_CASE(SUITE("UnsolRetryDelay"))
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg);
	t.app.EnableAutoSendCallback(false); // will respond with failure
	t.slave.OnLowerLayerUp();

	// check for the startup null unsol packet, but fail the transaction
	REQUIRE(t.Read() ==  "F0 82 80 00");
	REQUIRE(t.mts.NumActive() ==  1); // this should cause a timer to become active
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00");
}

TEST_CASE(SUITE("UnsolData"))
{
	SlaveConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	// do a transaction before the layer comes online to prove that the null transaction
	// is occuring before unsol data is sent
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	t.slave.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	// should immediately try to send another unsol packet,
	// Grp2Var1, qual 0x17, count 1, index 0, quality+val == 0x01
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 01");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent
}


TEST_CASE(SUITE("UnsolicitedStaysDisabledEvenIfDataAreLoadedPriorToOpen"))
{
SlaveConfig cfg; cfg.mDisableUnsol = true;
SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

{
Transaction tx(&t.db);
t.db.Update(Analog(0), 0);
}

t.slave.OnLowerLayerUp();

// Outstation shouldn't send an unsolicited handshake b/c unsol it disabled
REQUIRE(t.NothingToRead());
}

TEST_CASE(SUITE("UnsolicitedStaysDisabledEvenIfDataAreLoadedAfterOpen"))
{
SlaveConfig cfg; cfg.mDisableUnsol = true;
SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

auto pObs = t.slave.GetDataObserver();

t.slave.OnLowerLayerUp();

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
	SlaveConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	cfg.mEventMaxConfig.mMaxBinaryEvents = 2; // set the max to 2 to make testing easy
	cfg.mUnsolPackDelay = TimeDuration::Milliseconds(0);
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	// null unsol
	t.slave.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	// this transaction will overflow the event buffer
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
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
	SlaveConfig cfg;
	cfg.mMaxFragSize = 10; //this will cause the unsol response to get fragmented
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 2);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	t.slave.OnLowerLayerUp();
	REQUIRE(t.Read() ==  "F0 82 80 00");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 1);
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
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
	SlaveConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	//now send a write IIN request, and test that the slave answers immediately
	t.SendToSlave("C0 02 50 01 00 07 07 00");
	REQUIRE(t.Read() ==  "C0 81 00 00");

	t.slave.OnUnsolSendSuccess();
	REQUIRE(t.Count() ==  0);
}

TEST_CASE(SUITE("ReadDuringUnsol"))
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToSlave("C0 01 3C 02 06");

	t.slave.OnUnsolSendSuccess();
	REQUIRE(t.Read() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReadWriteDuringUnsol"))
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.DispatchOne());
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToSlave("C0 01 3C 01 06");

	//now send a write IIN request, and test that the slave answers immediately
	t.SendToSlave("C0 02 50 01 00 07 07 00");
	t.slave.OnUnsolSendSuccess();
	REQUIRE(t.Read() ==  "C0 81 00 00");
}

TEST_CASE(SUITE("UnsolEnable"))
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToSlave("C0 14 3C 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 00");

	// should automatically send the previous data as unsol
	REQUIRE(t.Read() ==  "F0 82 80 00 02 01 17 01 00 01");
}

TEST_CASE(SUITE("UnsolEnableBadObject"))
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	REQUIRE(t.Read() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToSlave("C0 14 01 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 01");
}

TEST_CASE(SUITE("UnsolEnableDisableFailure"))
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 14 3C 02 06");
	REQUIRE(t.Read() ==  "C0 81 80 01"); //FUNC_NOT_SUPPORTED
}
*/



