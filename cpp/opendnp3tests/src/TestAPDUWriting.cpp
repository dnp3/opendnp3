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

#include <opendnp3/APDUWriter.h>

#include <openpal/ToHex.h>
#include <opendnp3/objects/Group60.h>

using namespace openpal;
using namespace opendnp3;


BOOST_AUTO_TEST_SUITE(APDUWritingTestSuite)

BOOST_AUTO_TEST_CASE(AllObjectsAndRollback)
{
	uint8_t buffer[50];
	APDURequestWriter writer(WriteBuffer(buffer, 50));
	writer.SetControl(AppControlField(true, true, false, false, 0));
	writer.SetFunction(FunctionCode::READ);
	writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var2::ID, QualifierCode::ALL_OBJECTS);
	writer.Mark();
	writer.WriteHeader(Group60Var3::ID, QualifierCode::ALL_OBJECTS);
	writer.WriteHeader(Group60Var4::ID, QualifierCode::ALL_OBJECTS);

	{
		auto hex = toHex(writer.GetWritten().ToReadOnly());
		BOOST_REQUIRE_EQUAL("C0 01 3C 01 06 3C 02 06 3C 03 06 3C 04 06", hex);
	}

	writer.Rollback();

	{
		auto hex = toHex(writer.GetWritten().ToReadOnly());
		BOOST_REQUIRE_EQUAL("C0 01 3C 01 06 3C 02 06", hex);
	}
}

BOOST_AUTO_TEST_SUITE_END()
