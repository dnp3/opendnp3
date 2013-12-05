#include <boost/test/unit_test.hpp>

#include <iostream>
#include <boost/asio.hpp>
#include <functional>

#include <asiopal/DeadlineTimerSteadyClock.h>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::system;

void AssertCanceled(bool* apFlag, const boost::system::error_code& ec)
{
	if(ec) *apFlag = true;
}

void Cancel(monotonic_timer* aptimer)
{
	aptimer->cancel();
}

BOOST_AUTO_TEST_SUITE(TestBoostASIO)

BOOST_AUTO_TEST_CASE(TimerCancel)
{
	bool flag = false;

	io_service io;
	monotonic_timer t1(io, std::chrono::seconds(0));
	monotonic_timer t2(io, std::chrono::seconds(1));

	t1.async_wait(std::bind(Cancel, &t2));
	t2.async_wait(std::bind(AssertCanceled, &flag, std::placeholders::_1));

	io.run();

	BOOST_REQUIRE(flag);
}

BOOST_AUTO_TEST_SUITE_END()

