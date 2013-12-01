
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

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/CommandTask.h>
#include <opendnp3/CommandHelpers.h>
#include <asiopal/Log.h>

using namespace opendnp3;
using namespace openpal;
using namespace asiopal;

BOOST_AUTO_TEST_SUITE(CommandTaskTestSuite)

BOOST_AUTO_TEST_CASE(FullSequence)
{
	EventLog log;
	CommandTask ct(Logger(&log, LEV_INFO, "task"));
	CommandResponse rsp;
	auto formatter = [](APDU & arAPDU, FunctionCode aCode) {
		return CommandHelpers::ConfigureRequest(arAPDU, aCode, ControlRelayOutputBlock(CC_LATCH_ON), 0, Group12Var1::Inst());
	};
	auto responder = [&rsp](CommandResponse aRsp) {
		rsp = aRsp;
	};
	ct.Configure(formatter, responder);
	ct.AddCommandCode(FunctionCode::SELECT);

	APDU frag;
	ct.ConfigureRequest(frag);
	HexSequence hs("C0 81 00 00 0C 01 17 01 00 03 01 64 00 00 00 64 00 00 00 00");
	frag.Reset();
	frag.Write(hs);
	frag.Interpret();
	auto result = ct.OnFinalResponse(frag);
	BOOST_REQUIRE_EQUAL(TR_SUCCESS, result);
	BOOST_REQUIRE(CommandResponse::OK(CommandStatus::SUCCESS) == rsp);
}

BOOST_AUTO_TEST_SUITE_END()
