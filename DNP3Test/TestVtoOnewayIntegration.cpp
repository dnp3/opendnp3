
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


#include <APL/RandomizedBuffer.h>
#include <APL/LoggableMacros.h>
#include <APLTestTools/MockPhysicalLayerMonitor.h>

#include "VtoIntegrationTestBase.h"

#include <functional>

using namespace apl;
using namespace apl::dnp;
using namespace std::chrono;

class VtoOnewayTestStack : public VtoIntegrationTestBase
{
public:
	VtoOnewayTestStack(
	    bool clientOnSlave = true,
	    bool aImmediateOutput = false,
	    bool aLogToFile = false,
	    FilterLevel level = LEV_INFO,
	    boost::uint16_t port = MACRO_PORT_VALUE) :

		VtoIntegrationTestBase(clientOnSlave, aImmediateOutput, aLogToFile, level, port),
		local(mLog.GetLogger(level, "local-mock-phys-monitor"), &vtoClient, &timerSource, milliseconds(500), seconds(10)),
		remote(mLog.GetLogger(level, "remote-mock-phys-monitor"), &vtoServer, &timerSource, milliseconds(500), seconds(10)) {

	}

	virtual ~VtoOnewayTestStack() {
		local.Shutdown();
		remote.Shutdown();
	}

	bool WaitForBothSides(PhysicalLayerState aState, millis_t aTimeout = 30000) {
		return this->WaitForLocalState(aState) && this->WaitForRemoteState(aState);
	}

	bool WaitForLocalState(PhysicalLayerState aState, millis_t aTimeout = 30000) {
		LOG_BLOCK(LEV_EVENT, "Waiting for local state: " << ConvertPhysicalLayerStateToString(aState));
		return testObj.ProceedUntil(std::bind(&MockPhysicalLayerMonitor::NextStateIs, &local, aState), aTimeout);
	}

	bool WaitForRemoteState(PhysicalLayerState aState, millis_t aTimeout = 30000) {
		LOG_BLOCK(LEV_EVENT, "Waiting for remote state: " << ConvertPhysicalLayerStateToString(aState));
		return testObj.ProceedUntil(std::bind(&MockPhysicalLayerMonitor::NextStateIs, &remote, aState), aTimeout);
	}

	bool WaitForExpectedDataToBeReceived(millis_t aTimeout = 30000) {
		return testObj.ProceedUntil(std::bind(&MockPhysicalLayerMonitor::AllExpectedDataHasBeenReceived, &remote), aTimeout);
	}

	MockPhysicalLayerMonitor local;
	MockPhysicalLayerMonitor remote;
};


BOOST_AUTO_TEST_SUITE(VtoOnewayIntegrationSuite)

BOOST_AUTO_TEST_CASE(Reconnection)
{
	VtoOnewayTestStack stack(true, false, false);

	// start up everything, the local side should be able to open
	stack.remote.Start();
	stack.local.Start();

	RandomizedBuffer data(100);

	for(size_t i = 0; i < 1; ++i) {

		stack.Log(LOCATION, "Begin iteration - Waiting for both sides to open");

		BOOST_REQUIRE(stack.WaitForBothSides(PLS_OPEN));

		// test that data is correctly sent both ways

		data.Randomize();
		stack.local.ExpectData(data);
		stack.local.WriteData(data);
		stack.Log(LOCATION, "Waiting for expected data");
		BOOST_REQUIRE(stack.WaitForExpectedDataToBeReceived());

		// close the remote loopback server, which will cause both sides to close and reopen
		stack.remote.Close();
		stack.Log(LOCATION, "Waiting for both sides to close");
		BOOST_REQUIRE(stack.WaitForBothSides(PLS_CLOSED));
	}
}

BOOST_AUTO_TEST_CASE(RemoteSideOpenFailureBouncesLocalConnection)
{
	VtoOnewayTestStack test(true, false);

	BOOST_REQUIRE(test.WaitForLocalState(PLS_CLOSED));

	test.local.Start();

	for(size_t i = 0; i < 3; ++i) {
		// start local connection, we should immediately be able to connect to this side
		BOOST_REQUIRE(test.WaitForLocalState(PLS_OPEN));
		// since the remote side can't connect to the port we should have our local connection bounced
		BOOST_REQUIRE(test.WaitForLocalState(PLS_CLOSED));
	}
}

BOOST_AUTO_TEST_CASE(SocketIsClosedIfRemoteDrops)
{
	VtoOnewayTestStack stack(true, false, false);

	// start all components, should connect
	stack.remote.Start();
	stack.local.Start();

	for(size_t i = 0; i < 3; ++i) {
		BOOST_REQUIRE(stack.WaitForBothSides(PLS_OPEN));
		// kill remote connection, should kill our local connection
		stack.remote.Close();
		BOOST_REQUIRE(stack.WaitForBothSides(PLS_CLOSED));
	}
}

void TestLargeDataOneWay(VtoOnewayTestStack& arTest, size_t aSizeInBytes)
{	
	// start everything
	arTest.local.Start();
	arTest.remote.Start();

	BOOST_REQUIRE(arTest.WaitForBothSides(PLS_OPEN));

	// test that a large set of data flowing one way works
	RandomizedBuffer data(aSizeInBytes);
	arTest.remote.ExpectData(data);
	arTest.local.WriteData(data);
	BOOST_REQUIRE(arTest.WaitForExpectedDataToBeReceived(60000));

	// this will cause an exception if we receive any more data beyond what we wrote
	arTest.testObj.ProceedForTime(1000);
}

#define MACRO_BUFFER_SIZE 1<< 20 // 1 << 20 == 1MB, 1<<24 == 16MB

BOOST_AUTO_TEST_CASE(LargeDataTransferMasterToSlave)
{
	VtoOnewayTestStack stack(true, false, false);
	stack.tcpPipe.client.SetCorruptionProbability(0.005);	
	stack.tcpPipe.server.SetCorruptionProbability(0.005);
	TestLargeDataOneWay(stack, MACRO_BUFFER_SIZE);
}

BOOST_AUTO_TEST_CASE(LargeDataTransferSlaveToMaster)
{
	VtoOnewayTestStack stack(false, false, false);	
	stack.tcpPipe.client.SetCorruptionProbability(0.005);
	stack.tcpPipe.server.SetCorruptionProbability(0.005);
	TestLargeDataOneWay(stack, MACRO_BUFFER_SIZE);
}

BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
