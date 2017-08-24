/*
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

#include "TaskBehavior.h"

#include <limits>

using namespace openpal;

namespace opendnp3
{

TaskBehavior TaskBehavior::SingleExecutionNoRetry()
{
	return SingleExecutionNoRetry(MonotonicTimestamp::Max()); // no start expiration
}

TaskBehavior TaskBehavior::SingleExecutionNoRetry(const openpal::MonotonicTimestamp startExpiration)
{
	return TaskBehavior(
	           TimeDuration::Min(),			// not periodic
	           MonotonicTimestamp::Min(),		// run immediately
	           TimeDuration::Max(),
	           TimeDuration::Max(),
	           startExpiration
	       );
}

TaskBehavior TaskBehavior::ImmediatePeriodic(
    const openpal::TimeDuration& period,
    const openpal::TimeDuration& minRetryDelay,
    const openpal::TimeDuration& maxRetryDelay
)
{
	return TaskBehavior(
	           period,
	           MonotonicTimestamp::Min(),		// run immediately
	           minRetryDelay,
	           maxRetryDelay,
	           MonotonicTimestamp::Max()		// no start expiraion
	       );
}

TaskBehavior TaskBehavior::SingleImmediateExecutionWithRetry(
    const TimeDuration& minRetryDelay,
    const TimeDuration& maxRetryDelay
)
{
	return TaskBehavior(
	           TimeDuration::Min(),			// not periodic
	           MonotonicTimestamp::Min(),		// run immediatey
	           minRetryDelay,
	           maxRetryDelay,
	           MonotonicTimestamp::Max()
	       );
}

TaskBehavior TaskBehavior::ReactsToIINOnly()
{
	return TaskBehavior(
	           TimeDuration::Min(),			// not periodic
	           MonotonicTimestamp::Max(),		// only run when needed
	           TimeDuration::Max(),			// never retry
	           TimeDuration::Max(),
	           MonotonicTimestamp::Max()
	       );
}

TaskBehavior::TaskBehavior(
    const TimeDuration& period,
    const MonotonicTimestamp& expiration,
    const TimeDuration& minRetryDelay,
    const TimeDuration& maxRetryDelay,
    const openpal::MonotonicTimestamp& startExpiration
) :
	period(period),
	expiration(expiration),
	minRetryDelay(minRetryDelay),
	maxRetryDelay(maxRetryDelay),
	currentRetryDelay(minRetryDelay),
	startExpiration(startExpiration)
{}


void TaskBehavior::OnSuccess(const openpal::MonotonicTimestamp& now)
{
	this->currentRetryDelay = this->minRetryDelay;
	this->expiration = this->period.IsNegative() ? openpal::MonotonicTimestamp::Max() : now.Add(this->period);
}

void TaskBehavior::OnResponseTimeout(const MonotonicTimestamp& now)
{
	this->expiration = now.Add(this->currentRetryDelay);
	this->currentRetryDelay = this->CalcNextRetryTimeout();
}

void TaskBehavior::Reset()
{
	this->disabled = false;
	this->expiration = MonotonicTimestamp::Min();
	this->currentRetryDelay = this->minRetryDelay;
}

void TaskBehavior::Disable()
{
	this->disabled = true;
	this->expiration = MonotonicTimestamp::Max();
}

TimeDuration TaskBehavior::CalcNextRetryTimeout() const
{
	const bool doubling_would_cause_mult_overflow = this->currentRetryDelay.milliseconds >= (std::numeric_limits<int64_t>::max() / 2);

	const auto doubled = doubling_would_cause_mult_overflow ? this->maxRetryDelay : TimeDuration::Milliseconds(2 * this->currentRetryDelay.milliseconds);

	return (doubled > this->maxRetryDelay) ? this->maxRetryDelay : doubled;
}

}



