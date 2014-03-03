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
#include "MockLogSubscriber.h"
#include "SlaveTestObject.h"

#include <opendnp3/DNPConstants.h>
#include <opendnp3/Util.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace boost;

BOOST_AUTO_TEST_SUITE(OutstationTestSuite)

BOOST_AUTO_TEST_CASE(InitialState)
{
	SlaveConfig cfg;
	SlaveTestObject t(cfg, DatabaseTemplate());

	t.slave.OnLowerLayerDown();
	
	t.slave.OnSolSendSuccess();
	BOOST_REQUIRE_EQUAL(SERR_INVALID_STATE, t.log.NextErrorCode());
	t.slave.OnUnsolSendSuccess();
	BOOST_REQUIRE_EQUAL(SERR_INVALID_STATE, t.log.NextErrorCode());
	t.slave.OnSolFailure();
	BOOST_REQUIRE_EQUAL(SERR_INVALID_STATE, t.log.NextErrorCode());
	t.slave.OnUnsolFailure();
	BOOST_REQUIRE_EQUAL(SERR_INVALID_STATE, t.log.NextErrorCode());
	t.slave.OnRequest(APDURecord(), SequenceInfo());
	BOOST_REQUIRE_EQUAL(SERR_INVALID_STATE, t.log.NextErrorCode());
}

BOOST_AUTO_TEST_CASE(TimersCancledOnClose)
{
	SlaveConfig cfg; cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();
	t.slave.OnLowerLayerDown();

	//timer for time
	BOOST_REQUIRE_EQUAL(t.mts.NumActive(), 0);
}

BOOST_AUTO_TEST_CASE(DataPost)
{

	SlaveConfig cfg;
	SlaveTestObject t(cfg, DatabaseTemplate::BinaryOnly(1));
	
	t.db.staticData.binaries.metadata[0].clazz = PC_CLASS_1;

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
	SlaveTestObject t(cfg, DatabaseTemplate::BinaryOnly(1));	
	t.db.staticData.binaries.metadata[0].clazz = PC_CLASS_1;

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
	SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

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
	SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(1));

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
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();


	t.SendToSlave("C0 10"); // func = initialize application (16)
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // IIN = device restart + func not supported
}

BOOST_AUTO_TEST_CASE(WriteIIN)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 07 07 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 00 00");
}

BOOST_AUTO_TEST_CASE(WriteIINEnabled)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 07 07 01");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 04");
}

BOOST_AUTO_TEST_CASE(WriteIINWrongBit)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 50 01 00 04 04 01");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 04");
}

BOOST_AUTO_TEST_CASE(WriteNonWriteObject)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 01 02 00 07 07 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01");
}


BOOST_AUTO_TEST_CASE(DelayMeasure)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 17"); //delay measure
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 34 02 07 01 00 00"); // response, Grp51Var2, count 1, value == 00 00
}

BOOST_AUTO_TEST_CASE(DelayMeasureExtraData)
{
	SlaveConfig cfg;  cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 17 DE AD BE EF"); //delay measure
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); // Func not supported
}

BOOST_AUTO_TEST_CASE(WriteTimeDate)
{
	SlaveConfig cfg; 
	cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 32 01 07 01 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
	BOOST_REQUIRE_EQUAL(t.mTimeWrites.size(), 1);	
	BOOST_REQUIRE_EQUAL(t.mTimeWrites.front().msSinceEpoch, 1234);
	
}

BOOST_AUTO_TEST_CASE(WriteTimeDateNotAsking)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = false;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 32 01 07 01 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 04"); // param error
	t.mts.DispatchOne();
	BOOST_REQUIRE_EQUAL(t.mTimeWrites.size(), 0);	
}

BOOST_AUTO_TEST_CASE(WriteTimeDateMultipleObjects)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mAllowTimeSync = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 02 32 01 07 02 D2 04 00 00 00 00 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 90 04"); // param error +  need time still set
	t.mts.DispatchOne();
	BOOST_REQUIRE_EQUAL(t.mTimeWrites.size(), 0);
}

BOOST_AUTO_TEST_CASE(BlankIntegrityPoll)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00");
}

BOOST_AUTO_TEST_CASE(ReadClass0MultiFrag)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	cfg.mMaxFragSize = 20; // override to use a fragment length of 20
	SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(8));	
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		for (uint16_t i = 0; i < 8; i++) t.db.Update(Analog(0, AQ_ONLINE), i);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0

	// Response should be (30,1)x2 per fragment, quality ONLINE, value 0
	// 4 fragment response, first 3 fragments should be confirmed, last one shouldn't be
	BOOST_REQUIRE_EQUAL(t.Read(), "A0 81 80 00 1E 01 00 00 01 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "20 81 80 00 1E 01 00 02 03 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "20 81 80 00 1E 01 00 04 05 01 00 00 00 00 01 00 00 00 00");
	BOOST_REQUIRE_EQUAL(t.Read(), "40 81 80 00 1E 01 00 06 07 01 00 00 00 00 01 00 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadFuncNotSupported)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate());
	t.slave.OnLowerLayerUp();

	t.SendToSlave("C0 01 0C 01 06"); //try to read 12/1 (control block)
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 01"); //restart/func not supported
}

void TestStaticRead(const std::string& arRequest, const std::string& arResponse)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate(1,1,1,1,1,1));	
	t.slave.OnLowerLayerUp();

	t.SendToSlave(arRequest);
	BOOST_REQUIRE_EQUAL(t.Read(), arResponse);
}

// ---- Static data reads ----- //

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

BOOST_AUTO_TEST_CASE(ReadByRangeHeader)
{
	SlaveConfig cfg;
	cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate::AnalogOnly(10));	
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(Analog(42, AQ_ONLINE), 5);
		t.db.Update(Analog(41, AQ_ONLINE), 6);
	}

	t.SendToSlave("C0 01 1E 02 00 05 06"); // read 30 var 2, [05 : 06]
	BOOST_REQUIRE_EQUAL(t.Read(), "C0 81 80 00 1E 02 00 05 06 01 2A 00 01 29 00");
}

template <class PointType>
void TestStaticType(const SlaveConfig& aCfg, const DatabaseTemplate& tmp, PointType aVal, const std::string& aRsp)
{
	SlaveTestObject t(aCfg, tmp);
	
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(PointType(aVal), 0);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), aRsp);
}

template <class T>
void TestStaticCounter(StaticCounterResponse aRsp, T aValue, const std::string& arRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true; cfg.mStaticCounter = aRsp;
	TestStaticType<Counter>(cfg, DatabaseTemplate::CounterOnly(1), aValue, arRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var1)
{
	TestStaticCounter(StaticCounterResponse::Group20Var1, 5, "C0 81 80 00 14 01 00 00 00 01 05 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var2)
{
	TestStaticCounter(StaticCounterResponse::Group20Var2, 5, "C0 81 80 00 14 02 00 00 00 01 05 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var5)
{
	TestStaticCounter(StaticCounterResponse::Group20Var5, 5, "C0 81 80 00 14 05 00 00 00 05 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp20Var6)
{
	TestStaticCounter(StaticCounterResponse::Group20Var6, 5, "C0 81 80 00 14 06 00 00 00 05 00");
}

template <class T>
void TestStaticAnalog(StaticAnalogResponse aRsp, T aVal, const std::string& arRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true; cfg.mStaticAnalog = aRsp;
	TestStaticType<Analog>(cfg, DatabaseTemplate::AnalogOnly(1), aVal, arRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var2)
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var2, 100, "C0 81 80 00 1E 02 00 00 00 01 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var3)
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var3, 65536, "C0 81 80 00 1E 03 00 00 00 00 00 01 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var4)
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var4, 100, "C0 81 80 00 1E 04 00 00 00 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var5)
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var5, 95.6, "C0 81 80 00 1E 05 00 00 00 01 33 33 BF 42");
}

BOOST_AUTO_TEST_CASE(ReadGrp30Var6)
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var6, -20, "C0 81 80 00 1E 06 00 00 00 01 00 00 00 00 00 00 34 C0");
}

template <class T>
void TestStaticBinaryOutputStatus(T aVal, const std::string& aRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true;
	SlaveTestObject t(cfg, DatabaseTemplate(0, 0, 0, 0, 1, 0));
	t.slave.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(BinaryOutputStatus(aVal, TQ_ONLINE), 0);
	}

	t.SendToSlave("C0 01 3C 01 06"); // Read class 0
	BOOST_REQUIRE_EQUAL(t.Read(), aRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp10Var2)
{
	TestStaticBinaryOutputStatus<bool>(true, "C0 81 80 00 0A 02 00 00 00 81");
}

template <class T>
void TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse aRsp, T aVal, const string& arRsp)
{
	SlaveConfig cfg; cfg.mDisableUnsol = true; cfg.mStaticAnalogOutputStatus = aRsp;
	TestStaticType<AnalogOutputStatus>(cfg, DatabaseTemplate::AnalogOutputStatusOnly(1), aVal, arRsp);
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var1)
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var1, 100, "C0 81 80 00 28 01 00 00 00 01 64 00 00 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var2)
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var2, 100, "C0 81 80 00 28 02 00 00 00 01 64 00");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var3)
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var3, 95.6, "C0 81 80 00 28 03 00 00 00 01 33 33 BF 42");
}

BOOST_AUTO_TEST_CASE(ReadGrp40Var4)
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var4, -20.0, "C0 81 80 00 28 04 00 00 00 01 00 00 00 00 00 00 34 C0");
}

BOOST_AUTO_TEST_SUITE_END()

