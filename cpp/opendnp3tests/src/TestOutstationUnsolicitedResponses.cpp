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
#include "APDUHexBuilders.h"

#include <opendnp3/DNPErrorCodes.h>


using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationUnsolicitedTestSuite - " name

TEST_CASE(SUITE("NullUnsolOnStartup"))
{
	OutstationConfig cfg;  cfg.params.allowUnsolicited = true;
	OutstationTestObject t(cfg);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany());

	// Null UNSOL, FIR, FIN, CON, UNS, w/ restart and need-time IIN
	REQUIRE(t.lower.PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
}

TEST_CASE(SUITE("UnsolRetryDelay"))
{
	OutstationConfig cfg; cfg.params.allowUnsolicited = true;
	OutstationTestObject t(cfg);	
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany());

	// check for the startup null unsol packet, but fail the transaction
	REQUIRE(t.lower.PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
	t.outstation.OnSendResult(true);

	REQUIRE(t.exe.NumPendingTimers() ==  1); // confirm timer
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany()); // expire the timer

	REQUIRE(t.lower.NumWrites() == 0);
	REQUIRE(t.exe.NumPendingTimers() == 1); // unsol retry timer
	REQUIRE(t.exe.AdvanceToNextTimer());
	REQUIRE(t.exe.RunMany()); // expire the timer

	// repeats the null unsol packet with the next sequence #
	REQUIRE(t.lower.PopWriteAsHex() == hex::NullUnsolicited(1, IINField(IINBit::DEVICE_RESTART)));
}

TEST_CASE(SUITE("UnsolData"))
{
	OutstationConfig cfg; cfg.params.allowUnsolicited = true;
	cfg.params.unsolClassMask = ALL_CLASSES; // allows us to skip the "enable unsol" step
	OutstationTestObject t(cfg, DatabaseTemplate::BinaryOnly(3), EventBufferConfig::AllTypes(5));
		
	t.db.staticData.binaries.metadata[0].clazz = CLASS_1;
	t.db.staticData.binaries.metadata[1].clazz = CLASS_2;
	t.db.staticData.binaries.metadata[2].clazz = CLASS_3;
	
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany());
	
	REQUIRE(t.lower.PopWriteAsHex() ==  hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
	t.outstation.OnSendResult(true);
	t.SendToOutstation(hex::UnsolConfirm(0));

	// do a transaction before the layer comes online to prove that the null transaction
	// is occuring before unsol data is sent
	{
		Transaction tr(t.db);
		t.db.Update(Binary(false, BQ_ONLINE), 2);
	}

	REQUIRE(t.exe.RunMany());	
	
	// should immediately try to send another unsol packet,
	// Grp2Var1, qual 0x17, count 1, index 2, quality+val == 0x01
	REQUIRE(t.lower.PopWriteAsHex() ==  "F1 82 80 00 02 01 28 01 00 02 00 01");
	t.outstation.OnSendResult(true);
	t.SendToOutstation(hex::UnsolConfirm(1));

	REQUIRE(t.exe.RunMany());
	REQUIRE(t.lower.NumWrites() == 0);	
}

TEST_CASE(SUITE("UnsolEventBufferOverflow"))
{
	OutstationConfig cfg;
	cfg.params.allowUnsolicited = true;
	cfg.params.unsolClassMask = CLASS_1;	
	OutstationTestObject t(cfg, DatabaseTemplate::BinaryOnly(1), EventBufferConfig(2));

	t.db.staticData.binaries.metadata[0].clazz = CLASS_1;
	
	// null unsol
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.exe.RunMany());

	REQUIRE(t.lower.PopWriteAsHex() == hex::NullUnsolicited(0, IINField(IINBit::DEVICE_RESTART)));
	t.outstation.OnSendResult(true);
	t.SendToOutstation(hex::UnsolConfirm(0));


	// this transaction will overflow the event buffer
	{
		Transaction tr(t.db);
		t.db.Update(Binary(true, BQ_ONLINE), 0);
		t.db.Update(Binary(false, BQ_ONLINE), 0);
		t.db.Update(Binary(true, BQ_ONLINE), 0);
	}

	REQUIRE(t.exe.RunMany());


	// should immediately try to send 2 unsol events
	// Grp2Var1, qual 0x17, count 2, index 0
	// The last two values should be published, 0x01 and 0x81 (false and true)
	// the first value is lost off the front of the buffer
	REQUIRE(t.lower.PopWriteAsHex() ==  "F1 82 80 08 02 01 28 02 00 00 00 01 00 00 81");

	//REQUIRE(t.lower.NumWrites() ==  0); //check that no more frags are sent
}

/*
TEST_CASE(SUITE("UnsolMultiFragments"))
{
	OutstationConfig cfg;
	cfg.mMaxFragSize = 10; //this will cause the unsol response to get fragmented
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 2);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);

	t.outstation.OnLowerLayerUp();
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00");

	REQUIRE(t.app.NumAPDU() ==  0); //check that no more frags are sent

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 1);
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.RunOne()); //dispatch the data update event

	REQUIRE(t.mts.NumActive() ==  1); // unsol pack timer should be active

	REQUIRE(t.mts.RunOne()); //dispatch the unsol pack timer

	// Only enough room to in the APDU to carry a single value
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 01 01");
	// should immediately try to send another unsol packet
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 00 01");
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

	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.RunOne());
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 00 81");

	//now send a write IIN request, and test that the outstation answers immediately
	t.SendToOutstation("C0 02 50 01 00 07 07 00");
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 00 00");

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

	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.RunOne());
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToOutstation("C0 01 3C 02 06");

	t.outstation.OnUnsolSendSuccess();
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 80 00");
}

TEST_CASE(SUITE("ReadWriteDuringUnsol"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero();
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00");

	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	REQUIRE(t.mts.RunOne());
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 00 81");

	t.SendToOutstation("C0 01 3C 01 06");

	//now send a write IIN request, and test that the outstation answers immediately
	t.SendToOutstation("C0 02 50 01 00 07 07 00");
	t.outstation.OnUnsolSendSuccess();
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 00 00");
}

TEST_CASE(SUITE("UnsolEnable"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.RunOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToOutstation("C0 14 3C 02 06");
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 80 00");

	// should automatically send the previous data as unsol
	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00 02 01 17 01 00 01");
}

TEST_CASE(SUITE("UnsolEnableBadObject"))
{
	OutstationConfig cfg; cfg.mUnsolPackDelay = TimeDuration::Zero(); cfg.mUnsolMask = ClassMask(false, false, false);
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	REQUIRE(t.lower.PopWriteAsHex() ==  "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.outstation.GetDataObserver());
		t.outstation.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	REQUIRE(t.mts.RunOne()); //dispatch the data update event
	REQUIRE(t.app.NumAPDU() ==  0); //check that no unsol packets are generated

	t.SendToOutstation("C0 14 01 02 06");
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 80 01");
}

TEST_CASE(SUITE("UnsolEnableDisableFailure"))
{
	OutstationConfig cfg; cfg.disableUnsol = true;
	OutstationTestObject t(cfg);
	t.db.Configure(MeasurementType::BINARY, 1);
	t.db.SetClass(MeasurementType::BINARY, CLASS_1);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 14 3C 02 06");
	REQUIRE(t.lower.PopWriteAsHex() ==  "C0 81 80 01"); //FUNC_NOT_SUPPORTED
}
*/



