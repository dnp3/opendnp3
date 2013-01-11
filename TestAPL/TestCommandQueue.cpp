//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <APL/CommandQueue.h>
#include <APLTestTools/MockCommandHandler.h>
#include <APLTestTools/MockResponseAcceptor.h>

using namespace apl;




BOOST_AUTO_TEST_SUITE(CommandQueueSuite)
BOOST_AUTO_TEST_CASE(SimpleWriteAndRead)
{
	CommandQueue cq;

	BinaryOutput bo;
	bo.mRawCode = CC_LATCH_ON;

	cq.AcceptCommand(bo, 3, 4, NULL);

	BOOST_REQUIRE_EQUAL(cq.Next(), CT_BINARY_OUTPUT);

	BinaryOutput bo2;
	bo2.mRawCode = CC_LATCH_OFF;
	CommandData info;

	cq.Read(bo2, info);

	BOOST_REQUIRE_EQUAL(cq.Next(), CT_NONE);

	BOOST_REQUIRE_EQUAL(info.mIndex, 3);
	BOOST_REQUIRE_EQUAL(info.mSequence, 4);
	BOOST_REQUIRE_EQUAL(bo2.mRawCode, CC_LATCH_ON);
}

BOOST_AUTO_TEST_CASE(ExecuteCommand)
{
	CommandQueue cq;
	MockCommandHandler mh;
	MockResponseAcceptor mr;

	cq.AcceptCommand(BinaryOutput(CC_PULSE), 0, 0, &mr);
	BOOST_REQUIRE_EQUAL(cq.Size(), 1);
	cq.ExecuteCommand(&mh);
	BOOST_REQUIRE_EQUAL(cq.Size(), 0);
	BOOST_REQUIRE_EQUAL(mh.num_bo, 1);
	BOOST_REQUIRE_EQUAL(mh.num_sp, 0);
	BOOST_REQUIRE_EQUAL(mr.NumResponses(), 1);

	cq.AcceptCommand(Setpoint(0), 0, 0, &mr);
	BOOST_REQUIRE_EQUAL(cq.Size(), 1);
	cq.ExecuteCommand(&mh);
	BOOST_REQUIRE_EQUAL(cq.Size(), 0);
	BOOST_REQUIRE_EQUAL(mh.num_bo, 1);
	BOOST_REQUIRE_EQUAL(mh.num_sp, 1);
	BOOST_REQUIRE_EQUAL(mr.NumResponses(), 2);

	BOOST_REQUIRE_EQUAL(mr.PopResponse().Response.mResult, CS_SUCCESS);
}

BOOST_AUTO_TEST_CASE(OrderMaintained)
{
	CommandQueue cq;
	size_t testLength = 100;
	for(size_t i = 0; i < testLength; i++) {
		int type = rand() % 3;
		int seq = static_cast<int>(i);
		if(type == 0) {
			BinaryOutput bo;
			bo.mRawCode = (i % 2 == 0) ? CC_LATCH_ON : CC_LATCH_OFF;
			bo.mCount = (uint8_t)i % 255;
			cq.AcceptCommand(bo, i, seq, NULL);
		}
		else {
			Setpoint st;
			st.SetValue(static_cast<int32_t>(i));
			cq.AcceptCommand(st, i, seq, NULL);
		}
	}
	for(size_t i = 0; i < testLength; i++) {
		CommandTypes type = cq.Next();
		CommandData info;
		switch(type) {
		case CT_BINARY_OUTPUT: {
				BinaryOutput bo;
				cq.Read(bo, info);
				BOOST_REQUIRE_EQUAL(bo.mCount, i);
			} break;
		case CT_SETPOINT: {
				Setpoint st;
				cq.Read(st, info);
				BOOST_REQUIRE_EQUAL(st.GetIntValue(), (int)i);
			} break;
		case CT_NONE:
			BOOST_REQUIRE(false);
		}
		BOOST_REQUIRE_EQUAL(info.mIndex, i);
		BOOST_REQUIRE_EQUAL(info.mSequence, (int)i);

	}
	BOOST_REQUIRE_EQUAL(cq.Next(), CT_NONE);
}
BOOST_AUTO_TEST_SUITE_END()


