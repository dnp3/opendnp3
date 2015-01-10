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
#ifndef OPENDNP3_CONTROLSTATE_H
#define OPENDNP3_CONTROLSTATE_H


#include <openpal/executor/MonotonicTimestamp.h>


namespace opendnp3
{

///
/// Represent all of the mutable state for SBO controls
///
class ControlState
{
	
	public:	
	
	ControlState() : expectedSeq(0)		
	{}

	// Is this sequence number what's expected for the OPERATE
	bool IsOperateSequenceValid(uint8_t seqN) const
	{		
		return (seqN == expectedSeq);
	}

	bool IsSelectTimeValid(const openpal::MonotonicTimestamp& now, const openpal::TimeDuration& timeout) const
	{
		if (selectTime.milliseconds < now.milliseconds)
		{
			auto elapsed = now.milliseconds - selectTime.milliseconds;
			return elapsed < timeout;
		}
		else
		{
			return false;
		}		
	}

	void Select(uint8_t currentSeqN, const openpal::MonotonicTimestamp& now)
	{
		selectTime = now;
		expectedSeq = AppControlField::NextSeq(currentSeqN);
	}

	private:
	
	uint8_t expectedSeq;
	openpal::MonotonicTimestamp selectTime;	
	
};


}

#endif
