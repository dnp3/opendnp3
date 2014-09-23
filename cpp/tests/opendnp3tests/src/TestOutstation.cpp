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
#include "HexConversions.h"

#include <opendnp3/ErrorCodes.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "OutstationTestSuite - " name

TEST_CASE(SUITE("InitialState"))
{
	OutstationConfig config;
	OutstationTestObject test(config);
	
	test.LowerLayerDown();
	REQUIRE(flags::ERR == test.log.PopFilter());

	test.SendToOutstation("");
	REQUIRE(flags::ERR == test.log.PopFilter());

	test.OnSendResult(true);
	REQUIRE(flags::ERR == test.log.PopFilter());
}


TEST_CASE(SUITE("UnsupportedFunction"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 10"); // func = initialize application (16)
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + func not supported
}

TEST_CASE(SUITE("ApplicationIINBits"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();
	
	t.application.appIIN.deviceTrouble = true;
	t.application.appIIN.localControl = true;
	t.application.appIIN.configCorrupt = true;
	t.application.appIIN.needTime = true;

	t.SendToOutstation("C0 01"); // blank read
	// All 4 bits + restart
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 F0 20");
}

TEST_CASE(SUITE("ReadUnknownObject"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	// from the conformance tests, respond with IIN 2-1
	t.SendToOutstation("C0 01 00 00 06");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 02"); // IIN = device restart + unknown object
}

TEST_CASE(SUITE("ColdRestart"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	// try first with support turned off
	t.SendToOutstation("C0 0D");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + function not supported
	t.OnSendResult(true);

	t.application.coldRestartSupport = RestartMode::SUPPORTED_DELAY_FINE;
	t.application.coldRestartTimeDelay = 1;


	t.SendToOutstation("C1 0D");
	REQUIRE(t.lower.PopWriteAsHex() == "C1 81 80 00 34 02 07 01 01 00");
}

TEST_CASE(SUITE("WarmRestart"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	// try first with support turned off
	t.SendToOutstation("C0 0E");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + function not supported
	t.OnSendResult(true);

	t.application.warmRestartSupport = RestartMode::SUPPORTED_DELAY_COARSE;
	t.application.warmRestartTimeDelay = 65535;


	t.SendToOutstation("C1 0E");
	REQUIRE(t.lower.PopWriteAsHex() == "C1 81 80 00 34 01 07 01 FF FF");
}


TEST_CASE(SUITE("NoResponseToNoAckCodes"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	// outstation shouldn't respond to any of these
	std::vector<FunctionCode> codes;
	codes.push_back(FunctionCode::DIRECT_OPERATE_NR);
	codes.push_back(FunctionCode::FREEZE_AT_TIME_NR);
	codes.push_back(FunctionCode::FREEZE_CLEAR_NR);
	codes.push_back(FunctionCode::FREEZE_AT_TIME_NR);

	uint8_t sequence = 0;

	for (auto code : codes)
	{
		uint8_t bytes[2];		
		AppControlField control(true, true, false, false, sequence);
		bytes[0] = control.ToByte();
		bytes[1] = static_cast<uint8_t>(code);
		auto request = toHex(bytes, 2, true);

		t.SendToOutstation(request);
		REQUIRE(t.lower.PopWriteAsHex() == "");

		++sequence;
	}	
}

TEST_CASE(SUITE("WriteIIN"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation(hex::ClearRestartIIN(0));
	REQUIRE(t.lower.PopWriteAsHex() == hex::EmptyResponse(0));
}

TEST_CASE(SUITE("WriteIINEnabled"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 02 50 01 00 07 07 01");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteIINWrongBit"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 02 50 01 00 04 04 01");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteNonWriteObject"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 02 01 02 00 07 07 00");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01");
}

TEST_CASE(SUITE("DelayMeasure"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();	

	t.SendToOutstation("C0 17"); //delay measure
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00 34 02 07 01 00 00"); // response, Grp51Var2, count 1, value == 00 00
}

TEST_CASE(SUITE("DelayMeasureExtraData"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 17 DE AD BE EF"); //delay measure
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04"); // param error
}

TEST_CASE(SUITE("WriteTimeDate"))
{
	OutstationConfig config;	
	OutstationTestObject t(config);	
	t.LowerLayerUp();


	t.SendToOutstation("C1 02 32 01 07 01 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C1 81 80 00");
	REQUIRE(t.application.timestamps.size() == 1);
	REQUIRE(t.application.timestamps.front().msSinceEpoch == 1234);

}

TEST_CASE(SUITE("WriteTimeDateNotAsking"))
{
	OutstationConfig config;
	OutstationTestObject t(config);	
	t.LowerLayerUp();

	t.application.allowTimeWrite = false;
	t.SendToOutstation("C0 02 32 01 07 01 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04"); // param error	
	REQUIRE(t.application.timestamps.size() == 0);
}

TEST_CASE(SUITE("WriteTimeDateMultipleObjects"))
{
	OutstationConfig cfg;	
	OutstationTestObject t(cfg, DatabaseTemplate());
	t.LowerLayerUp();

	t.SendToOutstation("C0 02 32 01 07 02 D2 04 00 00 00 00 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04"); // param error +  need time still set	
	REQUIRE(t.application.timestamps.empty());
}

TEST_CASE(SUITE("BlankIntegrityPoll"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00");
}

TEST_CASE(SUITE("ReadClass0MultiFragAnalog"))
{
	OutstationConfig config;
	config.params.maxTxFragSize = 20; // override to use a fragment length of 20	
	OutstationTestObject t(config, DatabaseTemplate::AnalogOnly(8));
	t.LowerLayerUp();

	
	t.Transaction([](Database& db) {
		for (uint16_t i = 0; i < 8; i++)
		{
			db.Update(Analog(0, 0x01), i);
		}
	});
	

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

	// Response should be (30,1)x2 per fragment, quality ONLINE, value 0
	// 4 fragment response, first 3 fragments should be confirmed, last one shouldn't be
	REQUIRE(t.lower.PopWriteAsHex() == "A0 81 80 00 1E 01 00 00 01 01 00 00 00 00 01 00 00 00 00");
	t.OnSendResult(true);
	t.SendToOutstation("C0 00");
	REQUIRE(t.lower.PopWriteAsHex() == "21 81 80 00 1E 01 00 02 03 01 00 00 00 00 01 00 00 00 00");
	t.OnSendResult(true);
	t.SendToOutstation("C1 00");
	REQUIRE(t.lower.PopWriteAsHex() == "22 81 80 00 1E 01 00 04 05 01 00 00 00 00 01 00 00 00 00");
	t.OnSendResult(true);
	t.SendToOutstation("C2 00");
	REQUIRE(t.lower.PopWriteAsHex() == "43 81 80 00 1E 01 00 06 07 01 00 00 00 00 01 00 00 00 00");	
	t.OnSendResult(true);
	t.SendToOutstation("C3 00");

	REQUIRE(t.lower.PopWriteAsHex() == "");
}

TEST_CASE(SUITE("ReadFuncNotSupported"))
{
	OutstationConfig config;
	OutstationTestObject t(config);
	t.LowerLayerUp();

	t.SendToOutstation("C0 01 0C 01 06"); //try to read 12/1 (control block)
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); //restart/func not supported
}


void NewTestStaticRead(const std::string& request, const std::string& response)
{	
	OutstationConfig config;
	OutstationTestObject t(config, DatabaseTemplate::AllTypes(1));	
	t.LowerLayerUp();

	t.SendToOutstation(request);
	REQUIRE(t.lower.PopWriteAsHex() == response);
}

// ---- Static data reads ----- //

TEST_CASE(SUITE("ReadGrp1Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 01 00 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp1Var2ViaIntegrity"))
{
	NewTestStaticRead("C0 01 01 02 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, packed format
}

TEST_CASE(SUITE("ReadGrp10Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 0A 00 06", "C0 81 80 00 0A 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp20Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 00 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var1ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 01 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var5ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 05 06", "C0 81 80 00 14 05 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 00 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var1ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 01 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var3ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 03 06", "C0 81 80 00 1E 03 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp40Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 28 00 06", "C0 81 80 00 28 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadByRangeHeader"))
{
	OutstationConfig config;	
	OutstationTestObject t(config, DatabaseTemplate::AnalogOnly(10));
	t.LowerLayerUp();

	t.Transaction([](Database& db){
		db.Update(Analog(42, 0x01), 5);
		db.Update(Analog(41, 0x01), 6);
	});	

	t.SendToOutstation("C2 01 1E 02 00 05 06"); // read 30 var 2, [05 : 06]
	REQUIRE(t.lower.PopWriteAsHex() == "C2 81 80 00 1E 02 00 05 06 01 2A 00 01 29 00");
}

template <class PointType>
void TestStaticType(const OutstationConfig& config, const DatabaseTemplate& tmp, PointType value, const std::string& rsp)
{
	OutstationTestObject t(config, tmp);

	t.LowerLayerUp();

	t.Transaction([value](Database& db) 
	{ 
		db.Update(PointType(value), 0); 
	});

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

	REQUIRE(t.lower.PopWriteAsHex() == rsp);
}

template <class T>
void TestStaticCounter(StaticCounterResponse rspType, T aValue, const std::string& response)
{
	OutstationConfig cfg; cfg.defaultStaticResponses.counter = rspType;
	TestStaticType<Counter>(cfg, DatabaseTemplate::CounterOnly(1), aValue, response);
}

TEST_CASE(SUITE("ReadGrp20Var1"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var1, 5, "C0 81 80 00 14 01 00 00 00 01 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var2"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var2, 5, "C0 81 80 00 14 02 00 00 00 01 05 00");
}

TEST_CASE(SUITE("ReadGrp20Var5"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var5, 5, "C0 81 80 00 14 05 00 00 00 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var6"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var6, 5, "C0 81 80 00 14 06 00 00 00 05 00");
}

template <class T>
void TestStaticAnalog(StaticAnalogResponse rspType, T aVal, const std::string& response)
{
	OutstationConfig cfg; cfg.defaultStaticResponses.analog = rspType;
	TestStaticType<Analog>(cfg, DatabaseTemplate::AnalogOnly(1), aVal, response);
}

TEST_CASE(SUITE("ReadGrp30Var2"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var2, 100, "C0 81 80 00 1E 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var3"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var3, 65536, "C0 81 80 00 1E 03 00 00 00 00 00 01 00");
}

TEST_CASE(SUITE("ReadGrp30Var4"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var4, 100, "C0 81 80 00 1E 04 00 00 00 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var5"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var5, 95.6, "C0 81 80 00 1E 05 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp30Var6"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var6, -20, "C0 81 80 00 1E 06 00 00 00 01 00 00 00 00 00 00 34 C0");
}

template <class T>
void TestStaticBinaryOutputStatus(T value, const std::string& response)
{
	OutstationConfig cfg;
	OutstationTestObject t(cfg, DatabaseTemplate::BinaryOutputStatusOnly(1));
	t.LowerLayerUp();

	t.Transaction([value](Database& db) { db.Update(BinaryOutputStatus(value, 0x01), 0); });

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
	REQUIRE(t.lower.PopWriteAsHex() == response);
}

TEST_CASE(SUITE("ReadGrp10Var2"))
{
	TestStaticBinaryOutputStatus<bool>(true, "C0 81 80 00 0A 02 00 00 00 81");
}

template <class T>
void TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse aRsp, T aVal, const string& response)
{
	OutstationConfig cfg; cfg.defaultStaticResponses.analogOutputStatus = aRsp;
	TestStaticType<AnalogOutputStatus>(cfg, DatabaseTemplate::AnalogOutputStatusOnly(1), aVal, response);
}

TEST_CASE(SUITE("ReadGrp40Var1"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var1, 100, "C0 81 80 00 28 01 00 00 00 01 64 00 00 00");
}

TEST_CASE(SUITE("ReadGrp40Var2"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var2, 100, "C0 81 80 00 28 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp40Var3"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var3, 95.6, "C0 81 80 00 28 03 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp40Var4"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var4, -20.0, "C0 81 80 00 28 04 00 00 00 01 00 00 00 00 00 00 34 C0");
}
