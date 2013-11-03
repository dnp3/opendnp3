
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
// you under the terms of the License.
//

#include <boost/test/unit_test.hpp>

#include <opendnp3/Log.h>
#include <opendnp3/ToHex.h>
#include <opendnp3/EnhancedVtoRouter.h>
#include <opendnp3/VtoRouterSettings.h>
#include <opendnp3/VtoWriter.h>
#include <opendnp3/IVtoEventAcceptor.h>

#include "BufferHelpers.h"
#include "LogTester.h"
#include "MockPhysicalLayerAsync.h"
#include "MockExecutor.h"
#include "TestHelpers.h"

using namespace std;
using namespace opendnp3;
using namespace openpal;

class VtoRouterTestClassBase : public IVtoEventAcceptor
{
public:
	VtoRouterTestClassBase(size_t aWriterSize) :
		log(),
		exe(),
		phys(Logger(&log, LEV_DEBUG, "phys"), &exe),
		writer(Logger(&log, LEV_DEBUG, "writer"), aWriterSize),
		pRouter(NULL) {
		exe.SetAutoPost(true);
	}

	void Update(const VtoData& arEvent, PointClass aClass, size_t aIndex) {
		VtoEvent evt(arEvent, aClass, aIndex);
		mQueue.push(evt);
	}

	bool Read(VtoEvent& arEvent) {
		if(mQueue.size() == 0) writer.Flush(this, 1);

		if(mQueue.size() > 0) {
			VtoEvent evt = mQueue.front();
			mQueue.pop();
			arEvent = evt;
			return true;
		}
		else return false;
	}

	void CheckLocalChannelConnectedMessage(bool connected) {
		BOOST_REQUIRE(writer.Size() > 0);
		VtoEvent vto;
		BOOST_REQUIRE(Read(vto));
		BOOST_REQUIRE_EQUAL(vto.mIndex, 255);
		BOOST_REQUIRE_EQUAL(vto.mValue.mpData[0], 88);
		BOOST_REQUIRE_EQUAL(vto.mValue.mpData[1], (connected ? 0 : 1));
	}

	void UpdateVtoData(const std::string& arHex) {
		HexSequence hs(arHex);
		VtoData data(hs.Buffer(), hs.Size());
		pRouter->OnVtoDataReceived(data);
	}

	void SetRemoteState(bool online) {
		VtoData data(online ? VTODT_REMOTE_OPENED : VTODT_REMOTE_CLOSED);
		pRouter->OnVtoDataReceived(data);
	}

	void CheckVtoData(const std::string& arData) {
		VtoEvent vto;
		BOOST_REQUIRE(Read(vto));
		BOOST_REQUIRE_EQUAL(88, vto.mIndex); // the channel id
		const std::string hex = toHex(vto.mValue.mpData, vto.mValue.GetSize(), true);
		BOOST_REQUIRE_EQUAL(arData, hex);
	}

	LogTester log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	VtoWriter writer;
	std::queue<VtoEvent> mQueue;
	EnhancedVtoRouter* pRouter;
};

class ServerVtoRouterTestClass : public VtoRouterTestClassBase
{
public:
	ServerVtoRouterTestClass(const VtoRouterSettings& arSettings = VtoRouterSettings(88, true, true), size_t aWriterSize = 100) :
		VtoRouterTestClassBase(aWriterSize),
		router(arSettings, Logger(&log, LEV_DEBUG, "router"), &writer, &phys) {
		pRouter = &router;
		writer.AddVtoCallback(&router);
	}

	ServerSocketVtoRouter router;
};

class ClientVtoRouterTestClass : public VtoRouterTestClassBase
{
public:
	ClientVtoRouterTestClass(const VtoRouterSettings& arSettings = VtoRouterSettings(88, true, true), size_t aWriterSize = 100) :
		VtoRouterTestClassBase(aWriterSize),
		router(arSettings, Logger(&log, LEV_DEBUG, "router"), &writer, &phys) {
		pRouter = &router;
		writer.AddVtoCallback(&router);
	}

	ClientSocketVtoRouter router;
};

BOOST_AUTO_TEST_SUITE(EnhancedVtoRouterTests)

uint8_t data[3] = { 0xA, 0xB, 0xC };
VtoData vtoData(data, 3);

BOOST_AUTO_TEST_CASE(Construction)
{
	ServerVtoRouterTestClass rtc;
}

BOOST_AUTO_TEST_CASE(ServerSendsMagicChannelLocalConnected)
{
	ServerVtoRouterTestClass rtc;

	BOOST_REQUIRE(rtc.phys.IsOpening());

	rtc.phys.SignalOpenSuccess();

	BOOST_REQUIRE(rtc.phys.IsReading());

	rtc.CheckLocalChannelConnectedMessage(true);

	rtc.phys.TriggerClose();

	BOOST_REQUIRE_EQUAL(rtc.phys.NumClose(), 1);

	rtc.CheckLocalChannelConnectedMessage(false);
}

BOOST_AUTO_TEST_CASE(ServerReceivingDataWhenRemoteIsClosedCausesNotification)
{
	ServerVtoRouterTestClass rtc;

	rtc.UpdateVtoData("01 02 03");
	rtc.CheckLocalChannelConnectedMessage(false);
}

void TestDuplicateRemoteOpenCausesLocalReconnect(VtoRouterTestClassBase& arTest)
{
	arTest.SetRemoteState(true);

	arTest.exe.Dispatch();
	BOOST_REQUIRE(arTest.phys.IsOpening());
	arTest.phys.SignalOpenSuccess();

	arTest.CheckLocalChannelConnectedMessage(true);

	arTest.SetRemoteState(true);
	BOOST_REQUIRE(arTest.phys.IsClosing());
	arTest.phys.TriggerClose();
	arTest.CheckLocalChannelConnectedMessage(false);
}

BOOST_AUTO_TEST_CASE(ServerDuplicateRemoteOpenCausesLocalReconnect)
{
	ServerVtoRouterTestClass rtc;
	TestDuplicateRemoteOpenCausesLocalReconnect(rtc);
}

BOOST_AUTO_TEST_CASE(ClientDuplicateRemoteOpenCausesLocalReconnect)
{
	ClientVtoRouterTestClass rtc;
	TestDuplicateRemoteOpenCausesLocalReconnect(rtc);
}

BOOST_AUTO_TEST_CASE(ClientStartsOpeningAfterRemoteConnection)
{
	ClientVtoRouterTestClass rtc;
	BOOST_REQUIRE(!rtc.phys.IsOpening());

	BOOST_REQUIRE(!rtc.phys.IsOpening());

	rtc.SetRemoteState(true);

	BOOST_REQUIRE(rtc.phys.IsOpening());

	rtc.SetRemoteState(false);

	BOOST_REQUIRE(rtc.phys.IsClosing());
}

BOOST_AUTO_TEST_CASE(ClientSendsMagicChannelLocalConnected)
{
	ClientVtoRouterTestClass rtc;

	BOOST_REQUIRE_FALSE(rtc.phys.IsOpening());

	rtc.SetRemoteState(true);

	BOOST_REQUIRE(rtc.phys.IsOpening());
	rtc.phys.SignalOpenSuccess();

	rtc.CheckLocalChannelConnectedMessage(true);

	rtc.phys.TriggerRead("01 02 03 04 05");
	rtc.phys.TriggerRead("06 07 08 09 0A");

	rtc.phys.TriggerClose();

	BOOST_REQUIRE_EQUAL(rtc.phys.NumClose(), 1);

	rtc.CheckVtoData("01 02 03 04 05");
	rtc.CheckVtoData("06 07 08 09 0A");
	rtc.CheckLocalChannelConnectedMessage(false);
}


BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
