
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

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/high_resolution_timer.hpp>

#include <opendnp3/Types.h>
#include <functional>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace opendnp3;


void AssertCanceled(bool* apFlag, const boost::system::error_code& ec)
{
	if(ec) *apFlag = true;
}

void Cancel(high_resolution_timer* aptimer)
{
	aptimer->cancel();
}

BOOST_AUTO_TEST_SUITE(TestBoostASIO)

BOOST_AUTO_TEST_CASE(TimerCancel)
{
	bool flag = false;

	io_service io;
	high_resolution_timer t1(io, std::chrono::seconds(0));
	high_resolution_timer t2(io, std::chrono::seconds(1));

	t1.async_wait(std::bind(Cancel, &t2));
	t2.async_wait(std::bind(AssertCanceled, &flag, std::placeholders::_1));

	io.run();

	BOOST_REQUIRE(flag);
}

BOOST_AUTO_TEST_SUITE_END()

