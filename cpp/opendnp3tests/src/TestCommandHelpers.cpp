#include <boost/test/unit_test.hpp>

#include <opendnp3/CommandHelpers.h>
#include <opendnp3/Objects.h>

#include <openpal/ToHex.h>

#include "TestHelpers.h"
#include "BufferHelpers.h"

using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(CommandHelpersTestSuite)

BOOST_AUTO_TEST_CASE(ConfigurationAndValidation)
{
	APDU frag;
	ControlRelayOutputBlock crob(ControlCode::LATCH_ON);
	auto validator = CommandHelpers::ConfigureRequest<ControlRelayOutputBlock>(frag, FunctionCode::OPERATE, crob, 0, Group12Var1::Inst());
	

	BOOST_REQUIRE_EQUAL("C0 04 0C 01 17 01 00 03 01 64 00 00 00 64 00 00 00 00", toHex(frag.ToReadOnly(), true));

	APDU rsp;
	{
		HexSequence hs("C0 81 00 00 0C 01 17 01 00 03 01 64 00 00 00 64 00 00 00 00");
		rsp.Write(hs);
		rsp.Interpret();
	}
	auto status = validator(rsp);
	BOOST_REQUIRE(CommandStatus::SUCCESS == status);
}

BOOST_AUTO_TEST_SUITE_END()
