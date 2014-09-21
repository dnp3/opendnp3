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
#ifndef OPENPAL_TIMEDURATION_H
#define OPENPAL_TIMEDURATION_H

#include <cstdint>

namespace openpal
{

template <class T>
class TimeDurationBase
{

public:

	TimeDurationBase() : milliseconds(0)
	{}

	T GetMilliseconds() const
	{
		return milliseconds;
	}
	
	operator T() const { return milliseconds; }

protected:
	TimeDurationBase(T aMilliseconds) : milliseconds(aMilliseconds)
	{}

	T milliseconds;
};


/**
*  Strong typing for millisecond based time durations
*/
class TimeDuration : public TimeDurationBase<int64_t>
{

public:

	TimeDuration();

	bool IsPostive() const;

	static TimeDuration Min();

	static TimeDuration Zero();

	static TimeDuration Milliseconds(int64_t aMilliseconds);

	static TimeDuration Seconds(int64_t aSeconds);

	static TimeDuration Minutes(int64_t aMinutes);

private:
	TimeDuration(int64_t aMilliseconds);
};

bool operator==(const TimeDuration& lhs, const TimeDuration& rhs);

}

#endif
