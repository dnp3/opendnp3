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
#include <APL/Log.h>
#include <APL/LogToFile.h>
#include <APLTestTools/LogTester.h>
#include <APL/Exception.h>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;
using namespace apl;


#ifdef GetMessage
//to deal with that stupid windows GetMessageA define.
#undef GetMessage
#endif



BOOST_AUTO_TEST_SUITE(LogTest)

BOOST_AUTO_TEST_CASE( LogLifetimeTest )
{
	size_t aCount = 100;
	EventLog log;
	LogEntryCircularBuffer buff;
	log.AddLogSubscriber(&buff);
	Logger* pLogger = log.GetLogger(LEV_DEBUG, "LogTest");

	for ( size_t i = 0; i < aCount; i++ ) {
		pLogger->Log( apl::LEV_DEBUG, "Test test test test", "Test message" );
	}

	BOOST_REQUIRE_EQUAL( buff.Count(), aCount );

	LogEntry entry;
	for ( size_t i = 0; i < aCount; i++ ) {
		BOOST_REQUIRE(buff.ReadLog(entry));
		BOOST_REQUIRE_EQUAL( buff.Count(), aCount - i - 1);
	}
	BOOST_REQUIRE_EQUAL( buff.Count(), 0 );

}

BOOST_AUTO_TEST_CASE( SelectiveSubscription )
{
	EventLog log;
	LogEntryCircularBuffer buff;
	log.AddLogSubscriber(&buff, 10);

	LogEntry le1(LEV_WARNING, "foo", "bar", "msg", -1);
	LogEntry le2(LEV_WARNING, "foo", "bar", "msg", 10);
	LogEntry le3(LEV_WARNING, "foo", "bar", "msg", 2);

	log.Log(le1);
	BOOST_REQUIRE_EQUAL(0, buff.Count());
	log.Log(le2);
	BOOST_REQUIRE_EQUAL(1, buff.Count());
	log.Log(le3);
	BOOST_REQUIRE_EQUAL(1, buff.Count());
}

BOOST_AUTO_TEST_CASE( LogEntryParamsTest )
{
	EventLog log;
	LogEntryCircularBuffer buff;
	log.AddLogSubscriber(&buff);
	Logger* pLogger = log.GetLogger(LEV_DEBUG, "LogTest");

	BOOST_REQUIRE_EQUAL(buff.Count(), 0);
	pLogger->Log( LEV_DEBUG, "LogEntryParamsTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 1);

	LogEntry entry;
	BOOST_REQUIRE(buff.ReadLog(entry));

	BOOST_REQUIRE_EQUAL( entry.GetFilterLevel(), LEV_DEBUG );
	BOOST_REQUIRE_EQUAL( entry.GetLocation(), "LogEntryParamsTest" );
	BOOST_REQUIRE_EQUAL( entry.GetMessage(), "MessageMessage" );
	BOOST_REQUIRE_EQUAL( entry.GetErrorCode(), 5 );
	BOOST_REQUIRE_EQUAL( entry.GetDeviceName(), "LogTest");
}

BOOST_AUTO_TEST_CASE( LogLevelTest )
{
	EventLog log;
	LogEntryCircularBuffer buff;
	log.AddLogSubscriber(&buff);
	Logger* pLogger = log.GetLogger(LEV_INTERPRET, "LogTest");

	BOOST_REQUIRE_EQUAL(buff.Count(), 0);
	pLogger->Log( LEV_INFO, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 1);
	pLogger->Log( LEV_INTERPRET, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 2);
	pLogger->Log( LEV_COMM, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 2);
}

BOOST_AUTO_TEST_CASE( LogSubLevelTest )
{
	EventLog log;
	LogEntryCircularBuffer buff;
	log.AddLogSubscriber(&buff);
	Logger* pLogger = log.GetLogger(LEV_DEBUG, "LogTest")->GetSubLogger("LogTestSub", LEV_INTERPRET);

	BOOST_REQUIRE_EQUAL(buff.Count(), 0);
	pLogger->Log( LEV_INFO, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 1);
	pLogger->Log( LEV_INTERPRET, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 2);
	pLogger->Log( LEV_COMM, "LogLevelTest", "MessageMessage", 5 );
	BOOST_REQUIRE_EQUAL(buff.Count(), 2);
}

BOOST_AUTO_TEST_CASE( LogErrorCounting)
{
	apl::LogTester log(false);
	Logger* pLogger = log.mLog.GetLogger(LEV_DEBUG, "test1");
	pLogger->Log( LEV_DEBUG, "LogEntryParamsTest", "MessageMessage", 5 );

	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), 5);
	BOOST_REQUIRE_EQUAL(log.NextErrorCode(), -1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(LogToFileSuite)

BOOST_AUTO_TEST_CASE(AddRemove)
{

	{
		EventLog log;
		LogToFile file(&log, "unittest.log");
		Logger* pLogger = log.GetLogger(LEV_DEBUG, "test1");
		pLogger->Log( apl::LEV_DEBUG, "Test test test test", "Test message" );
	}

	ifstream inFile("unittest.log");
	std::string s;
	inFile >> s;
	BOOST_REQUIRE(s.find("Test test") != 0);

}

BOOST_AUTO_TEST_SUITE_END()
