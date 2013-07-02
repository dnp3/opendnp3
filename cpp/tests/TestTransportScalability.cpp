
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

#include "TransportScalabilityTestObject.h"

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/ProtocolUtil.h>
#include <opendnp3/Exception.h>

#include <functional>

using namespace std;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(AsyncTransportScalability)


BOOST_AUTO_TEST_CASE(TestSimpleSend)
{
	LinkConfig client(true, true);
	LinkConfig server(false, true);

	// Ubuntu and windows use different ephemeral port ranges...

#ifdef WIN32
	uint32_t port = 50000;
#else
	uint32_t port = 30000;
#endif

	// turned down the number of pairs for arm b/c of how long it takes to run.
#ifdef ARM
	uint16_t NUM_PAIRS = 50;
#else
	uint16_t NUM_PAIRS = 100;
#endif

	TransportScalabilityTestObject t(client, server, port, NUM_PAIRS);

	t.Start();

	BOOST_REQUIRE(t.ProceedUntil(std::bind(&TransportScalabilityTestObject::AllLayersUp, &t)));

	ByteStr b(2048, 0);

	t.SendToAll(b, b.Size());

	BOOST_REQUIRE(t.ProceedUntil(std::bind(&TransportScalabilityTestObject::AllLayerReceived, &t, b.Size()), 120000));
	BOOST_REQUIRE(t.AllLayerEqual(b, b.Size()));
}




BOOST_AUTO_TEST_SUITE_END()
