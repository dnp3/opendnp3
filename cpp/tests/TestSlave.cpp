
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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "MockLogSubscriber.h"
#include "SlaveTestObject.h"

#include <opendnp3/APDU.h>
#include <opendnp3/ObjectReadIterator.h>
#include <opendnp3/Util.h>

using namespace std;
using namespace opendnp3;
using namespace boost;


BOOST_AUTO_TEST_SUITE(SlaveSuite)

BOOST_AUTO_TEST_CASE(InitialState)
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg);

	APDU f;

	BOOST_REQUIRE_THROW(t.slave.OnLowerLayerDown(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.slave.OnSolSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.slave.OnUnsolSendSuccess(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.slave.OnSolFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.slave.OnUnsolFailure(), InvalidStateException);
	BOOST_REQUIRE_THROW(t.slave.OnRequest(f, SequenceInfo()), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(TimersCancledOnClose)
{
	SlaveConfig cfg; cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();
	t.slave.OnLowerLayerDown();

	//timer for time
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
}

BOOST_AUTO_TEST_CASE(DataPost)
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);

	t.db.SetClass(DT_BINARY, PC_CLASS_1);

	IDataObserver* pObs = t.slave.GetDataObserver();
	{
		Transaction t(pObs);
		Binary b(true, BQ_ONLINE);
		pObs->Update(b, 0);
	}

	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
}

BOOST_AUTO_TEST_CASE(DataPostToNonExistent)
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);

	IDataObserver* pObs = t.slave.GetDataObserver();
	{
		Transaction t(pObs);
		Binary b(true, BQ_ONLINE);
		pObs->Update(b, 5);
	}

	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);

	{
		Transaction t(pObs);
		Binary b(true, BQ_ONLINE);
		pObs->Update(b, 0);
	}
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1);
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
}

BOOST_AUTO_TEST_CASE(UnsolicitedStaysDisabledEvenIfDataAreLoadedPriorToOpen)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);

	auto pObs = t.slave.GetDataObserver();

	{
		Transaction t(pObs);
		pObs->Update(Analog(0), 0);
	}

	BOOST_REQUIRE(t.mts.Dispatch() > 0);

	t.slave.OnLowerLayerUp();

	// Outstation shouldn't send an unsolicited handshake b/c unsol it disabled
	BOOST_REQUIRE(t.NothingToRead());
}

BOOST_AUTO_TEST_CASE(UnsolicitedStaysDisabledEvenIfDataAreLoadedAfterOpen)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);

	auto pObs = t.slave.GetDataObserver();

	t.slave.OnLowerLayerUp();

	{
		Transaction t(pObs);
		pObs->Update(Analog(0), 0);
	}

	BOOST_REQUIRE(t.mts.Dispatch() > 0);

	// Outstation shouldn't send an unsolicited handshake b/c unsol it disabled
	BOOST_REQUIRE(t.NothingToRead());
}


BOOST_AUTO_TEST_CASE(UnsupportedFunction)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();


	t.SendToSlave("C0 10"); // func = initialize application (16)
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // IIN = device restart + func not supported
}

BOOST_AUTO_TEST_CASE(WriteIIN)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 07 07 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 00 00");
}

BOOST_AUTO_TEST_CASE(WriteIINEnabled)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 07 07 01");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 04");
}

BOOST_AUTO_TEST_CASE(WriteIINWrongBit)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 04 04 01");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 04");
}

BOOST_AUTO_TEST_CASE(WriteNonWriteObject)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 02 01 00 07 07 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01");
}

BOOST_AUTO_TEST_CASE(DelayMeasure)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 17"); //delay measure
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 34 02 07 01 00 00"); // response, Grp51Var2, count 1, value == 00 00
}

BOOST_AUTO_TEST_CASE(WriteTimeDate)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	MockLogSubscriber mls;
	t.mLog.AddLogSubscriber(&mls, TIME_SYNC_UPDATED);

	t.SendToSlave("C0 02 32 01 07 01 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.fakeTime.GetMillisecondsSinceEpoch(), 1234);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");

	BOOST_REQUIRE_EQUAL(1, mls.mEntries.size());
	int32_t utc;
	BOOST_REQUIRE(mls.mEntries.front().GetValue("MILLISEC_SINCE_EPOCH", utc));
	BOOST_REQUIRE_EQUAL(1234, utc);

}
BOOST_AUTO_TEST_CASE(WriteTimeDateNotAsking)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = false;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 32 01 07 01 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.fakeTime.GetMillisecondsSinceEpoch(), 0);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
}
BOOST_AUTO_TEST_CASE(WriteTimeDateMultipleObjects)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 32 01 07 02 D2 04 00 00 00 00 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.fakeTime.GetMillisecondsSinceEpoch(), 0);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 90 04");
}

BOOST_AUTO_TEST_CASE(BlankIntegrityPoll)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
}

BOOST_AUTO_TEST_CASE(BlankExceptionScan)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 3C 02 06"); // Read class 1
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
}

BOOST_AUTO_TEST_CASE(ReportVtoViaExceptionScan)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	IVtoWriter* pWriter = t.slave.GetVtoWriter();

	BOOST_REQUIRE_FALSE(t.mts.DispatchOne());

	uint8_t pData[3] = {0x13, 0x14, 0x15};
	pWriter->Write(pData, 3, 0xAA);

	BOOST_REQUIRE(t.mts.DispatchOne());

	t.SendToSlave("C0 01 3C 02 06"); // Read class 1

	// Slave should send 3 bytes of vto data with index AA and Group/Var 113/3
	BOOST_REQUIRE_EQUAL(t.Read(), "E0 81 80 00 71 03 17 01 AA 13 14 15");
}

BOOST_AUTO_TEST_CASE(ReportVtoViaUnsol)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	IVtoWriter* pWriter = t.slave.GetVtoWriter();

	BOOST_REQUIRE_FALSE(t.mts.DispatchOne());

	uint8_t pData[3] = {0x13, 0x14, 0x15};
	pWriter->Write(pData, 3, 0xAA);

	BOOST_REQUIRE(t.mts.DispatchOne());

	// Slave should send 3 bytes of vto data with index AA and Group/Var 113/3
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 71 03 17 01 AA 13 14 15");

	BOOST_REQUIRE_EQUAL(t.Count(), 0);
}

BOOST_AUTO_TEST_CASE(FlushesMoreVtoEventsAfterSuccess)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0; cfg.mEventMaxConfig.mMaxVtoEvents = 2;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	IVtoWriter* pWriter = t.slave.GetVtoWriter();

	BOOST_REQUIRE_FALSE(t.mts.DispatchOne());

	uint8_t pData[3] = {0x13, 0x14, 0x15};
	pWriter->Write(pData, 3, 0xAA);
	pWriter->Write(pData, 3, 0xAA);
	pWriter->Write(pData, 3, 0xAA); //write 3 seperate vto events

	BOOST_REQUIRE(t.mts.DispatchOne());

	// Slave should send the first two vto data blocks because that's all the event buffer holds
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 71 03 17 01 AA 13 14 15 71 03 17 01 AA 13 14 15");

	// After those two blocks succeed, the third block should be transmitted
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 71 03 17 01 AA 13 14 15");

	BOOST_REQUIRE_EQUAL(t.Count(), 0);
}

BOOST_AUTO_TEST_CASE(ReadClass0MultiFrag)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mMaxFragSize = 20; // override to use a fragment length of 20
	SlaveTestObject t(cfg);
	t.db.Configure(DT_ANALOG, 8);
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		for(size_t i = 0; i < 8; i++) t.db.Update(Analog(0, AQ_ONLINE), i);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0

	// Response should be (30,1)x2 per fragment, quality ONLINE, value 0
	// 4 fragment response, first 3 fragments should be confirmed, last one shouldn't be
	BOOST_REQUIRE_EQUAL(t.Read(), "A0 81 80 00 1E 01 00 00 01 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "20 81 80 00 1E 01 00 02 03 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "20 81 80 00 1E 01 00 04 05 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "40 81 80 00 1E 01 00 06 07 01 00 00 00 00 01 00 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadClass1)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);

	t.db.Configure(DT_ANALOG, 100);
	t.db.SetClass(DT_ANALOG, 0x10, PC_CLASS_1);
	t.db.SetClass(DT_ANALOG, 0x17, PC_CLASS_1);
	t.db.SetClass(DT_ANALOG, 0x05, PC_CLASS_1);
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

	/*
	 * The indices should be in reverse-order from how they were
	 * added, but the values for a given index should be in the same
	 * order.
	 */
	BOOST_REQUIRE_EQUAL(t.Read(), "E0 81 80 00 20 01 17 05 10 01 87 09 00 00 17 01 34 12 00 00 05 01 22 22 00 00 05 01 33 33 00 00 05 01 44 44 00 00");

	t.SendToSlave("C0 01 3C 02 06");			// Repeat read class 1
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");	// Buffer should have been cleared
}

BOOST_AUTO_TEST_CASE(ReadClass1TimeOrdered)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);

	t.db.Configure(DT_ANALOG, 100);
	t.db.SetClass(DT_ANALOG, 0x10, PC_CLASS_1);
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

		/*
		 * Expected order in packet should be:
		 * a2 -> a0 -> a3 -> a1
		 */
		t.db.Update(a0, 0x10);
		t.db.Update(a1, 0x10);
		t.db.Update(a2, 0x10);
		t.db.Update(a3, 0x10);
	}

	t.SendToSlave("C0 01 3C 02 06");

	/*
	 * The indices should be in reverse-order from how they were
	 * added, but the values for a given index should be in the same
	 * order.
	 */
	BOOST_REQUIRE_EQUAL(t.Read(), "E0 81 80 00 20 01 17 04 10 01 11 11 00 00 10 01 22 22 00 00 10 01 33 33 00 00 10 01 44 44 00 00");

	t.SendToSlave("C0 01 3C 02 06");			// Repeat read class 1
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");	// Buffer should have been cleared
}

BOOST_AUTO_TEST_CASE(NullUnsolOnStartup)
{
	SlaveConfig cfg;  cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Null UNSOL, FIR, FIN, CON, UNS, w/ restart and need-time IIN
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 90 00");
}

BOOST_AUTO_TEST_CASE(UnsolRetryDelay)
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg);
	t.app.EnableAutoSendCallback(false); //will respond with failure
	t.slave.OnLowerLayerUp();

	// check for the startup null unsol packet, but fail the transaction
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1); // this should cause a timer to become active
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");
}

BOOST_AUTO_TEST_CASE(UnsolData)
{
	SlaveConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);

	// do a transaction before the layer comes online to prove that the null transaction
	// is occuring before unsol data is sent
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	t.slave.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	// should immediately try to send another unsol packet,
	// Grp2Var1, qual 0x17, count 1, index 0, quality+val == 0x01
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 01");

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //check that no more frags are sent
}

BOOST_AUTO_TEST_CASE(UnsolEventBufferOverflow)
{
	SlaveConfig cfg;
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	cfg.mEventMaxConfig.mMaxBinaryEvents = 2; // set the max to 2 to make testing easy
	cfg.mUnsolPackDelay = 0;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);

	// null unsol
	t.slave.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	// this transaction will overflow the event buffer
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	// should immediately try to send 2 unsol events
	// Grp2Var1, qual 0x17, count 2, index 0
	// The last two values should be published, 0x01 and 0x81 (false and true)
	// the first value is lost off the front of the buffer
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 02 00 01 00 81");

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //check that no more frags are sent
}

BOOST_AUTO_TEST_CASE(UnsolMultiFragments)
{
	SlaveConfig cfg;
	cfg.mMaxFragSize = 10; //this will cause the unsol response to get fragmented
	cfg.mUnsolMask.class1 = true; // this allows the EnableUnsol sequence to be skipped
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 2);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);

	t.slave.OnLowerLayerUp();
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //check that no more frags are sent

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 1);
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the data update event

	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 1); // unsol pack timer should be active

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the unsol pack timer

	// Only enough room to in the APDU to carry a single value
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 01 01");
	// should immediately try to send another unsol packet
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 01");
}

// Test that non-read fragments are immediately responded to while waiting for a
// response to unsolicited data
BOOST_AUTO_TEST_CASE(WriteDuringUnsol)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0;
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 81");

	//now send a write IIN request, and test that the slave answers immediately
	t.SendToSlave("C0 02 50 01 00 07 07 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 00 00");

	t.slave.OnUnsolSendSuccess();
	BOOST_REQUIRE_EQUAL(t.Count(), 0);
}

BOOST_AUTO_TEST_CASE(ReadDuringUnsol)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0;
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 81");

	t.SendToSlave("C0 01 3C 02 06");

	t.slave.OnUnsolSendSuccess();
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
}

BOOST_AUTO_TEST_CASE(ReadWriteDuringUnsol)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0;
	cfg.mUnsolMask.class1 = true; //allows us to skip this step
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00");

	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(true, BQ_ONLINE), 0);
	}

	t.app.DisableAutoSendCallback();
	BOOST_REQUIRE(t.mts.DispatchOne());
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 81");

	t.SendToSlave("C0 01 3C 01 06");

	//now send a write IIN request, and test that the slave answers immediately
	t.SendToSlave("C0 02 50 01 00 07 07 00");
	t.slave.OnUnsolSendSuccess();
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 00 00");
}

BOOST_AUTO_TEST_CASE(SelectCROBNotSupported)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.cmdHandler.SetResponse(CS_NOT_SUPPORTED);

	// Select group 12 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00");
	std::string response = t.Read();
	BOOST_REQUIRE_EQUAL(response, "C0 81 80 04 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 04"); // 0x04 status == CS_NOT_SUPPORTED
}
BOOST_AUTO_TEST_CASE(SelectCROBTooMany)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mMaxControls = 1;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 12 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 0C 01 17 02 03 01 01 01 00 00 00 01 00 00 00 00 04 01 01 01 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 02 03 01 01 01 00 00 00 01 00 00 00 00 04 01 01 01 00 00 00 01 00 00 00 08"); // 0x08 status == CS_TOO_MANY_OPS
}

BOOST_AUTO_TEST_CASE(SelectOperateCROB)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 12 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00", SI_OTHER);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00"); // 0x00 status == CS_SUCCESS


	// operate
	t.SendToSlave("C1 04 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00", SI_CORRECT);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00");

}

BOOST_AUTO_TEST_CASE(SelectOperateCROBSameSequenceNumber)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(0, t.cmdHandler.mNumInvocations);

	// Select group 12 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00", SI_OTHER);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00"); // 0x00 status == CS_SUCCESS
	BOOST_REQUIRE_EQUAL(1, t.cmdHandler.mNumInvocations);


	// operate the first time with correct sequence #
	t.SendToSlave("C1 04 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00", SI_CORRECT);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(2, t.cmdHandler.mNumInvocations);


	// operate again with same sequence number, should respond success but not really do an operation
	t.SendToSlave("C1 04 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00", SI_PREV);
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 0C 01 17 01 03 01 01 01 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(2, t.cmdHandler.mNumInvocations);
}

BOOST_AUTO_TEST_CASE(SelectGroup41Var1)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 29 01 17 01 03 00 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 01 17 01 03 00 00 00 00 00");
}

BOOST_AUTO_TEST_CASE(SelectGroup41Var2)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 2, count = 1, index = 3
	t.SendToSlave("C0 03 29 02 17 01 03 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 02 17 01 03 00 00 00");
}

BOOST_AUTO_TEST_CASE(SelectGroup41Var3)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 3, count = 1, index = 1, value = 100.0
	t.SendToSlave("C0 03 29 03 17 01 01 00 00 C8 42 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 03 17 01 01 00 00 C8 42 00");
}

BOOST_AUTO_TEST_CASE(SelectGroup41Var4)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 4, count = 1, index = 1, value = 100.0
	t.SendToSlave("C0 03 29 04 17 01 01 00 00 00 00 00 00 59 40 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 04 17 01 01 00 00 00 00 00 00 59 40 00");
}



BOOST_AUTO_TEST_CASE(SelectOperateGroup41Var1)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C0 03 29 01 17 01 03 00 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 01 17 01 03 00 00 00 00 00"); // 0x00 status == CS_SUCCESS

	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C1 04 29 01 17 01 03 00 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 01 17 01 03 00 00 00 00 00"); // 0x00 status == CS_SUCCESS

}

BOOST_AUTO_TEST_CASE(SelectOperateGroup41Var2)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 2, count = 1, index = 3
	t.SendToSlave("C0 03 29 02 17 01 03 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 02 17 01 03 00 00 00"); // 0x00 status == CS_SUCCESS


	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C1 04 29 02 17 01 03 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 02 17 01 03 00 00 00"); // 0x00 status == CS_SUCCESS

}

BOOST_AUTO_TEST_CASE(SelectOperateGroup41Var3)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 3, count = 1, index = 1
	t.SendToSlave("C0 03 29 03 17 01 01 00 00 C8 42 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 03 17 01 01 00 00 C8 42 00"); // 0x00 status == CS_SUCCESS

	// operate group 41 Var 3, count = 1, index = 1
	t.SendToSlave("C1 04 29 03 17 01 01 00 00 C8 42 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 03 17 01 01 00 00 C8 42 00"); // 0x00 status == CS_SUCCESS
}

BOOST_AUTO_TEST_CASE(SelectOperateGroup41Var4)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 4, count = 1, index = 1
	t.SendToSlave("C0 03 29 04 17 01 01 00 00 00 00 00 00 59 40 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 04 17 01 01 00 00 00 00 00 00 59 40 00"); // 0x00 status == CS_SUCCESS


	// operate group 41 Var 4, count = 1, index = 1
	t.SendToSlave("C1 04 29 04 17 01 01 00 00 00 00 00 00 59 40 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 04 17 01 01 00 00 00 00 00 00 59 40 00"); // 0x00 status == CS_SUCCESS
}

BOOST_AUTO_TEST_CASE(DirectOperateGroup41Var1)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C1 05 29 01 17 01 03 00 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 01 17 01 03 00 00 00 00 00"); // 0x00 status == CS_SUCCESS
}
BOOST_AUTO_TEST_CASE(DirectOperateGroup41Var2)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select group 41 Var 1, count = 1, index = 3
	t.SendToSlave("C1 05 29 02 17 01 03 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 02 17 01 03 00 00 00"); // 0x00 status == CS_SUCCESS

}
BOOST_AUTO_TEST_CASE(DirectOperateGroup41Var3)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// operate group 41 Var 3, count = 1, index = 1
	t.SendToSlave("C1 05 29 03 17 01 01 00 00 C8 42 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 03 17 01 01 00 00 C8 42 00"); // 0x00 status == CS_SUCCESS

}
BOOST_AUTO_TEST_CASE(DirectOperateGroup41Var4)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// operate group 41 Var 4, count = 1, index = 1
	t.SendToSlave("C1 05 29 04 17 01 01 00 00 00 00 00 00 59 40 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 29 04 17 01 01 00 00 00 00 00 00 59 40 00"); // 0x00 status == CS_SUCCESS
}

BOOST_AUTO_TEST_CASE(SelectBadObject)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Select a binary input
	t.SendToSlave("C0 03 02 01 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // 0x04 status == CS_NOT_SUPPORTED
}

BOOST_AUTO_TEST_CASE(OperateBadObject)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Operate a binary input
	t.SendToSlave("C0 04 02 01 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // 0x04 status == CS_NOT_SUPPORTED
}
BOOST_AUTO_TEST_CASE(DirectOperateBadObject)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	// Operate a binary input
	t.SendToSlave("C0 05 02 01 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // 0x04 status == CS_NOT_SUPPORTED
}

BOOST_AUTO_TEST_CASE(UnsolEnable)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0; cfg.mUnsolMask = ClassMask(false, false, false);
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //check that no unsol packets are generated

	t.SendToSlave("C0 14 3C 02 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");

	// should automatically send the previous data as unsol
	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00 02 01 17 01 00 01");
}

BOOST_AUTO_TEST_CASE(UnsolEnableBadObject)
{
	SlaveConfig cfg; cfg.mUnsolPackDelay = 0; cfg.mUnsolMask = ClassMask(false, false, false);
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	BOOST_REQUIRE_EQUAL(t.Read(), "F0 82 80 00"); //Null UNSOL

	// do a transaction to show that unsol data is not being reported yet
	{
		Transaction tr(t.slave.GetDataObserver());
		t.slave.GetDataObserver()->Update(Binary(false, BQ_ONLINE), 0);
	}

	BOOST_REQUIRE(t.mts.DispatchOne()); //dispatch the data update event
	BOOST_REQUIRE_EQUAL(t.app.NumAPDU(), 0); //check that no unsol packets are generated

	t.SendToSlave("C0 14 01 02 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01");
}

BOOST_AUTO_TEST_CASE(UnsolEnableDisableFailure)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 14 3C 02 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); //FUNC_NOT_SUPPORTED
}

BOOST_AUTO_TEST_CASE(ReadFuncNotSupported)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 0C 01 06"); //try to read 12/1 (control block)
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); //restart/func not supported
}



void TestStaticRead(const std::string& arRequest, const std::string& arResponse)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.Configure(DT_ANALOG, 1);
	t.db.Configure(DT_COUNTER, 1);
	t.db.Configure(DT_CONTROL_STATUS, 1);
	t.db.Configure(DT_SETPOINT_STATUS, 1);
	t.slave.OnLowerLayerUp();

	t.SendToSlave(arRequest);
	BOOST_REQUIRE_EQUAL(t.Read(), arResponse);
}

/* ---- Static data reads ----- */


/*BOOST_AUTO_TEST_CASE(ReadGrp1Var1)
{
	SlaveConfig cfg; cfg.mStaticBinary = GrpVar(1,1);
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 9);
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 01 01 06");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 01 01 00 00 00 00 00"); // 1 byte start/stop, 2 bytes for bitfield with 9 members
}*/

BOOST_AUTO_TEST_CASE(ReadGrp1Var0ViaIntegrity)
{
	TestStaticRead("C0 01 01 00 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp1Var2ViaIntegrity)
{
	TestStaticRead("C0 01 01 02 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, packed format
}

BOOST_AUTO_TEST_CASE(ReadGrp10Var0ViaIntegrity)
{
	TestStaticRead("C0 01 0A 00 06", "C0 81 80 00 0A 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var0ViaIntegrity)
{
	TestStaticRead("C0 01 14 00 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(RreadGrp20Var1ViaIntegrity)
{
	TestStaticRead("C0 01 14 01 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(RreadGrp20Var5ViaIntegrity)
{
	TestStaticRead("C0 01 14 05 06", "C0 81 80 00 14 05 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var0ViaIntegrity)
{
	TestStaticRead("C0 01 1E 00 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var1ViaIntegrity)
{
	TestStaticRead("C0 01 1E 01 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var3ViaIntegrity)
{
	TestStaticRead("C0 01 1E 03 06", "C0 81 80 00 1E 03 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var0ViaIntegrity)
{
	TestStaticRead("C0 01 28 00 06", "C0 81 80 00 28 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}



// test that asking for a specific data type returns the requested type
void TestEventRead(const std::string& arRequest, const std::string& arResponse)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, 1);
	t.db.Configure(DT_ANALOG, 1);
	t.db.Configure(DT_COUNTER, 1);
	t.db.Configure(DT_CONTROL_STATUS, 1);
	t.db.Configure(DT_SETPOINT_STATUS, 1);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
	t.db.SetClass(DT_ANALOG, PC_CLASS_1);
	t.db.SetClass(DT_COUNTER, PC_CLASS_1);
	t.slave.OnLowerLayerUp();


	{
		Transaction tr(&t.db);
		t.db.Update(Binary(false, BQ_ONLINE), 0);
		t.db.Update(Counter(0, CQ_ONLINE), 0);
		t.db.Update(Analog(0.0, AQ_ONLINE), 0);
		t.db.Update(ControlStatus(false, TQ_ONLINE), 0);
		t.db.Update(SetpointStatus(0.0, PQ_ONLINE), 0);
	}

	t.SendToSlave(arRequest);
	BOOST_REQUIRE_EQUAL(t.Read(), arResponse);
}

BOOST_AUTO_TEST_CASE(ReadGrp2Var0)
{
	TestEventRead("C0 01 02 00 06", "E0 81 80 00 02 01 17 01 00 01"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(ReadGrp22Var0)
{
	TestEventRead("C0 01 16 00 06", "E0 81 80 00 16 01 17 01 00 01 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(ReadGrp32Var0)
{
	TestEventRead("C0 01 20 00 06", "E0 81 80 00 20 01 17 01 00 01 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(ReadGrp2Var1)
{
	TestEventRead("C0 01 02 01 06", "E0 81 80 00 02 01 17 01 00 01"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(ReadGrp2Var2)
{
	TestEventRead("C0 01 02 02 06", "E0 81 80 00 02 02 17 01 00 01 00 00 00 00 00 00"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(ReadGrp2Var3)
{
	TestEventRead("C0 01 02 03 06", "E0 81 80 00 33 01 07 01 00 00 00 00 00 00 02 03 17 01 00 01 00 00"); // 1 byte count == 1, ONLINE quality
}

BOOST_AUTO_TEST_CASE(InvalidObject)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.slave.OnLowerLayerUp();

	t.slave.OnUnknownObject();
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 02");
}


BOOST_AUTO_TEST_CASE(ComplexReadSequence)
{
	const size_t NUM = 4;
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_BINARY, NUM);
	t.db.SetClass(DT_BINARY, PC_CLASS_1);
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
	BOOST_REQUIRE_EQUAL(t.Read(), rsp);
}

BOOST_AUTO_TEST_CASE(ReadByRangeHeader)
{
	SlaveConfig cfg;
	cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_ANALOG, 10);
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(Analog(42, AQ_ONLINE), 5);
		t.db.Update(Analog(41, AQ_ONLINE), 6);
	}

	t.SendToSlave("C0 01 1E 02 00 05 06"); // read 30 var 2, [05 : 06]
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 1E 02 00 05 06 01 2A 00 01 29 00");
}

template <class PointType, class T>
void TestStaticType(SlaveConfig& aCfg, GrpVar& aGrpVar, int aGroup, int aVar, T aVal, const std::string& aRsp)
{
	aGrpVar = GrpVar(aGroup, aVar);
	SlaveTestObject t(aCfg);
	t.db.Configure(PointType::MeasEnum, 1);
	t.db.SetClass(PointType::MeasEnum, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(PointType(aVal, PointType::ONLINE), 0);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), aRsp);
}

template <class T>
void TestStaticCounter(int aVar, T aVal, const std::string& aRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	TestStaticType<Counter>(cfg, cfg.mStaticCounter, 20, aVar, aVal, aRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var1)
{
	TestStaticCounter(1, 5, "C0 81 80 00 14 01 00 00 00 01 05 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var2)
{
	TestStaticCounter(2, 5, "C0 81 80 00 14 02 00 00 00 01 05 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var5)
{
	TestStaticCounter(5, 5, "C0 81 80 00 14 05 00 00 00 05 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var6)
{
	TestStaticCounter(6, 5, "C0 81 80 00 14 06 00 00 00 05 00");
}

template <class T>
void TestStaticAnalog(int aVar, T aVal, const std::string& aRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	TestStaticType<Analog>(cfg, cfg.mStaticAnalog, 30, aVar, aVal, aRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var2)
{
	TestStaticAnalog(2, 100, "C0 81 80 00 1E 02 00 00 00 01 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var3)
{
	TestStaticAnalog(3, 65536, "C0 81 80 00 1E 03 00 00 00 00 00 01 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var4)
{
	TestStaticAnalog(4, 100, "C0 81 80 00 1E 04 00 00 00 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var5)
{
	TestStaticAnalog(5, 95.6, "C0 81 80 00 1E 05 00 00 00 01 33 33 BF 42");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var6)
{
	TestStaticAnalog(6, -20, "C0 81 80 00 1E 06 00 00 00 01 00 00 00 00 00 00 34 C0");
}

template <class T>
void TestStaticControlStatus(int aVar, T aVal, const std::string& aRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg);
	t.db.Configure(DT_CONTROL_STATUS, 1);
	t.db.SetClass(DT_CONTROL_STATUS, PC_CLASS_1);
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(ControlStatus(aVal, TQ_ONLINE), 0);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), aRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp10Var2)
{
	TestStaticControlStatus(2, true, "C0 81 80 00 0A 02 00 00 00 81");
}


template <class T>
void TestStaticSetpointStatus(int aVar, T aVal, const string& aRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	TestStaticType<SetpointStatus>(cfg, cfg.mStaticSetpointStatus, 40, aVar, aVal, aRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var1)
{
	TestStaticSetpointStatus(1, 100, "C0 81 80 00 28 01 00 00 00 01 64 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var2)
{
	TestStaticSetpointStatus(2, 100, "C0 81 80 00 28 02 00 00 00 01 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var3)
{
	TestStaticSetpointStatus(3, 95.6, "C0 81 80 00 28 03 00 00 00 01 33 33 BF 42");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var4)
{
	TestStaticSetpointStatus(4, -20.0, "C0 81 80 00 28 04 00 00 00 01 00 00 00 00 00 00 34 C0");
}




BOOST_AUTO_TEST_SUITE_END()

