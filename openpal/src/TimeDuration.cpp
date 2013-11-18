
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
// you under the terms of the License.
//

#include <openpal/TimeDuration.h>

#include <limits>

namespace openpal
{
	
TimeDuration TimeDuration::Min()
{
	return TimeDuration(std::numeric_limits<int64_t>::min());
}

TimeDuration TimeDuration::Zero()
{
	return TimeDuration(0);
}

TimeDuration TimeDuration::Milliseconds(int64_t aMilliseconds)
{
	return TimeDuration(aMilliseconds);
}

TimeDuration TimeDuration::Seconds(int64_t aSeconds)
{
	return TimeDuration(1000*aSeconds);
}	

TimeDuration TimeDuration::Minutes(int64_t aMinutes)
{
	return TimeDuration(1000*60*aMinutes);
}
	
TimeDuration::TimeDuration() : TimeDurationBase(0) {}

TimeDuration::TimeDuration(int64_t aMilliseconds) : TimeDurationBase(aMilliseconds)
{}

bool operator==(const TimeDuration& lhs, const TimeDuration& rhs)
{
	return lhs.GetMilliseconds() == rhs.GetMilliseconds();
}

}
