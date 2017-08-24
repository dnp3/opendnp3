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
#ifndef OPENDNP3_TIMESYNCSTATE_H
#define OPENDNP3_TIMESYNCSTATE_H

#include "opendnp3/app/AppSeqNum.h"

#include "openpal/executor/MonotonicTimestamp.h"


namespace opendnp3
{

///
/// Represent all of the mutable state for time synchornization
///
class TimeSyncState
{

public:

	TimeSyncState() {}

	void RecordCurrentTime(const AppSeqNum& seq, const openpal::MonotonicTimestamp& now)
	{
		valid = true;
		time = now;
		expectedSeqNum = seq.Next();
	}

	bool CalcTimeDifference(const AppSeqNum& seq, const openpal::MonotonicTimestamp& now)
	{
		if (!valid) return false;
		if (!expectedSeqNum.Equals(seq)) return false;
		if (now.milliseconds < time.milliseconds) return false;

		this->difference = openpal::TimeDuration::Milliseconds(now.milliseconds - time.milliseconds);
		this->valid = false;

		return true;
	}

	openpal::TimeDuration GetDifference() const
	{
		return this->difference;
	}

private:

	bool valid = false;
	openpal::MonotonicTimestamp time;
	openpal::TimeDuration difference;
	AppSeqNum expectedSeqNum;
};


}

#endif
