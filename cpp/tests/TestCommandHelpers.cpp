
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
#include <APLTestTools/TestHelpers.h>
#include <DNP3/CommandHelpers.h>
#include <DNP3/Objects.h>
#include <APL/ToHex.h>
#include <APLTestTools/BufferHelpers.h>

using namespace apl;
using namespace apl::dnp;


BOOST_AUTO_TEST_SUITE(CommandHelpersTestSuite)

BOOST_AUTO_TEST_CASE(ConfigurationAndValidation)
{
	APDU frag;
	ControlRelayOutputBlock crob(ControlCode::CC_LATCH_ON);
	auto validator = CommandHelpers::ConfigureRequest<ControlRelayOutputBlock>(frag, FC_OPERATE, crob, 0, Group12Var1::Inst());
	
	std::string hex = toHex(frag.GetBuffer(), frag.Size(), true);
	
	BOOST_REQUIRE_EQUAL(hex, "C0 04 0C 01 17 01 00 03 01 64 00 00 00 64 00 00 00 00");

	APDU rsp;
	{
		HexSequence hs("C0 81 00 00 0C 01 17 01 00 03 01 64 00 00 00 64 00 00 00 00");
		rsp.Write(hs, hs.Size());
		rsp.Interpret();
	}
	CommandStatus status = validator(rsp);
	BOOST_REQUIRE_EQUAL(CS_SUCCESS, status);
}

BOOST_AUTO_TEST_SUITE_END()