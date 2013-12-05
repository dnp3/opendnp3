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
	CommandTask ct(Logger(&log, LogLevel::Info, "task"));
	CommandResponse rsp;
	auto formatter = [](APDU & arAPDU, FunctionCode aCode) {
		return CommandHelpers::ConfigureRequest(arAPDU, aCode, ControlRelayOutputBlock(ControlCode::LATCH_ON), 0, Group12Var1::Inst());
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
