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
#ifndef __STARTUP_TASKS_H_
#define __STARTUP_TASKS_H_


#include <openpal/IUTCTimeSource.h>

#include "MasterTaskBase.h"


namespace opendnp3
{

class ITimeSource;

// Clears the outstation IIN restart bit
class ClearRestartIIN : public SimpleRspBase
{
public:
	ClearRestartIIN(openpal::Logger& arLogger);

	void ConfigureRequest(APDURequest& request);

	std::string Name() const
	{
		return "ClearRestartIIN";
	}
};

// Enables or disables unsolicited reporting
class ConfigureUnsol : public SimpleRspBase
{
public:
	ConfigureUnsol(openpal::Logger&);

	void Set(bool aIsEnable, int aClassMask);

	void ConfigureRequest(APDURequest& request);

	std::string Name() const
	{
		return "ConfigureUnsol";
	}

private:
	bool mIsEnable;
	int mClassMask;
};

// Synchronizes the time on the outstation
class TimeSync : public SingleRspBase
{
public:
	TimeSync(openpal::Logger&, openpal::IUTCTimeSource*);

	// override Init
	void Init();
	void ConfigureRequest(APDURequest& request);
	TaskResult _OnFinalResponse(const APDUResponseRecord&);

	std::string Name() const
	{
		return "TimeSync";
	}

private:
	openpal::IUTCTimeSource* mpTimeSrc;

	// < 0 implies the delay measure hasn't happened yet
	int64_t mDelay;

	// what time we sent the delay meas
	openpal::UTCTimestamp mStart;
};

} //ens ns

#endif

