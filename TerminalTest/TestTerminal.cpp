#define BOOST_TEST_MODULE terminal
#include <boost/test/unit_test.hpp>

#include <APL/Log.h>
#include <APL/Thread.h>
#include <APL/Location.h>

#include <APLTestTools/TestHelpers.h>
#include <APLTestTools/MockExecutor.h>
#include <APLTestTools/MockPhysicalLayerAsyncTS.h>

#include <iostream>

#include <Terminal/Terminal.h>
#include <Terminal/LogTerminalExtension.h>

using namespace std;
using namespace apl;

/*
tests that does a pretty stupid run through the Terminal UI commands to make sure
that everything is still functioning. This is not really intended to be a full validation
just a sanity check to make sure all the functionality has been run and an easy fast way
to develop new functionality.
*/
BOOST_AUTO_TEST_SUITE(UISuite)

bool SendAndTest(MockPhysicalLayerAsyncTS* apLayer, std::string aCmd, std::string aSearchString)
{
	aCmd.append("\r\n");
	apLayer->WriteToLayer(reinterpret_cast<const boost::uint8_t*>(aCmd.c_str()), aCmd.length());
	apLayer->Advance();
	return apLayer->BufferContains(aSearchString);
}

void TestCommandParsing(MockPhysicalLayerAsyncTS* apLayer)
{
	//check that a bad command string is parsed and ignored correclty
	BOOST_REQUIRE(SendAndTest(apLayer, "badCommand", "Unrecognized"));

	//check that a valid command with a space before or after the string is ignored
	BOOST_REQUIRE(SendAndTest(apLayer, "help log ", "Flushes"));
	BOOST_REQUIRE(SendAndTest(apLayer, " help log", "Flushes"));

	//test that 2 lines recieved at once are both processed
	BOOST_REQUIRE(SendAndTest(apLayer, "help\r\nhelp log", "Flushes"));

	//generate a big long string that should flood the Readline() buffer.
	uint8_t tooBigBuff[1100];
	for(int i = 0; i < 1100; ++i) tooBigBuff[i] = '0' + (i % 10);
	string tooBigString((char*)tooBigBuff, 1100);
	//add onto the end of the string a valid command we can check the output of
	tooBigString.append("\r\nlog print");
}

void TestHelpCommands(MockPhysicalLayerAsyncTS* apLayer)
{
	//test that help generates a big usage text screen.
	BOOST_REQUIRE(SendAndTest(apLayer, "help", "usage"));
	//test that we have atleast some subtopics for print
	BOOST_REQUIRE(SendAndTest(apLayer, "help log", "Flushes"));
	//make sure we get a usefull error message for a missing subtopic
	BOOST_REQUIRE(SendAndTest(apLayer, "help log faketopic", "No topic found"));
	//see that we appear to get the list of registered devices
	BOOST_REQUIRE(SendAndTest(apLayer, "help loggers", "all of the loggers"));

	//check that we get a list of sub commands (log should be one)
	BOOST_REQUIRE(SendAndTest(apLayer, "help log", "run"));

	//shows that atleast filter is a subcommand of set
	BOOST_REQUIRE(SendAndTest(apLayer, "help filter", "set"));
	//check that the help string is approriate for set filter
	BOOST_REQUIRE(SendAndTest(apLayer, "help filter", "filters"));
	//check that the help string is approriate for set col
	BOOST_REQUIRE(SendAndTest(apLayer, "help logcol", "column order"));

	//check that a bad subcommand generates the usage message.
	BOOST_REQUIRE(SendAndTest(apLayer, "loggers NotARealDevice", "usage: "));
}

void TestPrintCommands(MockPhysicalLayerAsyncTS* apLayer, Logger* apLogger)
{
	//check that our new device is in the list
	BOOST_REQUIRE(SendAndTest(apLayer, "loggers", "TestDevice"));

	//Log a string and see that it appears in the print log command
	apLogger->Log(LEV_ERROR, LOCATION, "TestLogOutput");
	BOOST_REQUIRE(SendAndTest(apLayer, "log print", "TestLogOutput"));

	//log another string and make sure that it is displayed if we ask for jsut the TestDevice
	apLogger->Log(LEV_ERROR, LOCATION, "TestOutputOnly");
	BOOST_REQUIRE(SendAndTest(apLayer, "log print TestDevice", "TestOutputOnly"));
}

void TestSetCommands(MockPhysicalLayerAsyncTS* apLayer)
{
	//test that the help string is correct
	BOOST_REQUIRE(SendAndTest(apLayer, "logcol", "usage: "));
	//test that we stop the user from having the same column more that once
	BOOST_REQUIRE(SendAndTest(apLayer, "logcol tt", "more than once"));
	//check that bad column names are thrown out.
	BOOST_REQUIRE(SendAndTest(apLayer, "logcol x", "Unrecognized log column"));
	//make sure a valid string is allowed
	BOOST_REQUIRE(SendAndTest(apLayer, "logcol fldm", ">"));

	//test the help string
	BOOST_REQUIRE(SendAndTest(apLayer, "filter", "usage: "));

	//check that a bad filter is ignored
	BOOST_REQUIRE(SendAndTest(apLayer, "filter x", "Couldn't parse"));
	//check that a bad filter is ignored, even if valid filters are present
	BOOST_REQUIRE(SendAndTest(apLayer, "filter dxp", "Couldn't parse"));
	//test that all works
	BOOST_REQUIRE(SendAndTest(apLayer, "filter a", ">"));
	//test that none works
	BOOST_REQUIRE(SendAndTest(apLayer, "filter n", ">"));
	//test that setting all of the filters manually works
	BOOST_REQUIRE(SendAndTest(apLayer, "filter dciwev", ">"));

	//leave it set to all to error only
	BOOST_REQUIRE(SendAndTest(apLayer, "filter e", ">"));

	//make sure it ignores attempts to set filters on bad devieces
	BOOST_REQUIRE(SendAndTest(apLayer, "filter dciwev BadDevice", "Unrecognized device"));
	//check that it works for a good device
	BOOST_REQUIRE(SendAndTest(apLayer, "filter dciwev TestDevice", ">"));
}

void TestRunCommands(MockPhysicalLayerAsyncTS* apLayer, Logger* apLogger)
{
	//put an entry into the log that we will pickup on RunLog so we see something
	apLogger->Log(LEV_ERROR, LOCATION, "TestOutputOnRun");

	//do the run command, notice I adedd a \r\n which forces a second line of input to be ready
	//immediateley to break the "run" cycle after a single iteration (this is necessary since the
	//parser is expecting ">" which we don't get during a "run" command.
	BOOST_REQUIRE(SendAndTest(apLayer, "log run\r\n", "TestOutputOnRun"));
}

BOOST_AUTO_TEST_CASE(TerminalInteractions)
{
	//prepare the terminal on a "non-standard" port
	EventLog log;
	Logger* pLoggerA = log.GetLogger(LEV_INTERPRET, "Terminal");

	MockExecutor mts;
	MockPhysicalLayerAsyncTS phys(log.GetLogger(LEV_INTERPRET, "Phys"), &mts);

	LogTerminalExtension lte(&log);
	Terminal trm(pLoggerA, &phys, &mts, "Test Terminal", false);
	trm.AddExtension(&lte);
	//trm.Init();

	BOOST_REQUIRE(mts.DispatchOne());

	//add a "device" to the logger so we can test the devices behavior
	Logger* logger = log.GetLogger(LEV_ERROR, "TestDevice");

	TestCommandParsing(&phys);
	TestHelpCommands(&phys);
	TestPrintCommands(&phys, logger);

	TestSetCommands(&phys);
	TestRunCommands(&phys, logger);
}

BOOST_AUTO_TEST_SUITE_END()

