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

#ifndef __DEADLINE_TIMER_STEADY_CLOCK_H_
#define __DEADLINE_TIMER_STEADY_CLOCK_H_

#include <openpal/Types.h>
#include <boost/asio.hpp>

namespace boost
{
namespace asio
{

/*
Extension to boost::asio to allow use of a monotonic clock via boost::chrono::steady_clock with the
asio::basic_deadline_timer.  Monotonic clocks are immune to adjustments of the system clock.
The underlying clock and time_type are chrono::steady_clock, but the durations are in posix_time::time_duration
in order to achieve a uniform and familiar interface.
*/
template <>
struct time_traits<openpal::timer_clock> {
	/**
	The underlying time type is based on chrono::steady_clock's time_point.
	*/
	typedef openpal::timer_clock::time_point time_type;

	/**
	But the duration is based upon posix_time's time_duration, just like the ASIO built-in deadline_timer.
	*/
	typedef openpal::timer_clock::duration duration_type;

	/**
	Accessing the clock yields the current time from the steady_clock.
	*/
	static time_type now() {
		return openpal::timer_clock::now();
	}

	/**
	Form a new time point (in Chrono) by adding in a time_duration (from posix_time), using the thing they have in
	common -- microseconds.
	*/
	static time_type add(const time_type& time, const duration_type& duration) {
		return time + duration;
	}

	/**
	Subtract two time points (in Chrono) to yield a time_duration (from posix_time), using the thing they have in
	common -- microseconds.
	*/
	static duration_type subtract(const time_type& timeLhs, const time_type& timeRhs) {
		return timeLhs - timeRhs;
	}

	/**
	Test whether one chrono time is less than another.
	*/
	static bool less_than(const time_type& timeLhs, const time_type& timeRhs) {
		return timeLhs < timeRhs;
	}

	/**
	Convert to posix_time::time_duration type, since this is what ASIO needs under the hood.  Since we're already
	representing durations in posix_time::time_duration, this is a mere pass-through.
	*/
	static boost::posix_time::time_duration to_posix_duration(const duration_type& duration) {
		auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		return boost::posix_time::microseconds(us.count());
	}
};

/**
Convenience typedef for pairing the steady_clock time_traits (above) with a basic_deadline_timer yielding
a ready-to-use deadline timer that is based on a monotonic time that is immune to system clock changes.
A Boosty name has been chosen, as it is expected that in the near future boost::asio will have this built in.
*/
typedef boost::asio::basic_deadline_timer<openpal::timer_clock> monotonic_timer;

}
}

#endif

