//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __STARTUP_TASKS_H_
#define __STARTUP_TASKS_H_

#include <APL/Types.h>
#include "MasterTaskBase.h"

#include <chrono>

namespace apl
{
class ITimeSource;
}

namespace apl
{
namespace dnp
{

// Clears the outstation IIN restart bit
class ClearRestartIIN : public SimpleRspBase
{
public:
	ClearRestartIIN(Logger*);

	void ConfigureRequest(APDU& arAPDU);
	std::string Name() const {
		return "ClearRestartIIN";
	}
};

// Enables or disables unsolicited reporting
class ConfigureUnsol : public SimpleRspBase
{
public:
	ConfigureUnsol(Logger*);

	void Set(bool aIsEnable, int aClassMask);

	void ConfigureRequest(APDU& arAPDU);
	std::string Name() const {
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
	TimeSync(Logger*, ITimeSource*);

	// override Init
	void Init();
	void ConfigureRequest(APDU& arAPDU);
	TaskResult _OnFinalResponse(const APDU&);

	std::string Name() const {
		return "TimeSync";
	}

private:
	ITimeSource* mpTimeSrc;

	// < 0 implies the delay measure hasn't happened yet
	millis_t mDelay;

	// what time we sent the delay meas
	std::chrono::high_resolution_clock::time_point mStart;
};

}
} //ens ns

#endif
