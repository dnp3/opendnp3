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
#include "LogTester.h"

#include <asiopal/Log.h>

#include <openpal/Exception.h>

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;
using namespace opendnp3;
using namespace openpal;

BOOST_AUTO_TEST_SUITE(LogTest)

BOOST_AUTO_TEST_CASE( LogErrorCounting)
{
	LogTester log;
	Logger logger(&log, LogLevel::Debug, "test1");
	logger.Log( LogLevel::Debug, "LogEntryParamsTest", "MessageMessage", 5 );

	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), 5);
	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), -1);
}

BOOST_AUTO_TEST_SUITE_END()
