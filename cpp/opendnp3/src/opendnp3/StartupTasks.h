
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
#ifndef __STARTUP_TASKS_H_
#define __STARTUP_TASKS_H_

#include <openpal/Visibility.h>
#include <openpal/IUTCTimeSource.h>

#include "MasterTaskBase.h"


namespace opendnp3
{

class ITimeSource;

// Clears the outstation IIN restart bit
class DLL_LOCAL ClearRestartIIN : public SimpleRspBase
{
public:
	ClearRestartIIN(openpal::Logger& arLogger);

	void ConfigureRequest(APDU& arAPDU);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "ClearRestartIIN";
	}
#endif
};

// Enables or disables unsolicited reporting
class DLL_LOCAL ConfigureUnsol : public SimpleRspBase
{
public:
	ConfigureUnsol(openpal::Logger&);

	void Set(bool aIsEnable, int aClassMask);

	void ConfigureRequest(APDU& arAPDU);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "ConfigureUnsol";
	}
#endif

private:
	bool mIsEnable;
	int mClassMask;
};

// Synchronizes the time on the outstation
class DLL_LOCAL TimeSync : public SingleRspBase
{
public:
	TimeSync(openpal::Logger&, openpal::IUTCTimeSource*);

	// override Init
	void Init();
	void ConfigureRequest(APDU& arAPDU);
	TaskResult _OnFinalResponse(const APDU&);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string Name() const {
		return "TimeSync";
	}
#endif

private:
	openpal::IUTCTimeSource* mpTimeSrc;

	// < 0 implies the delay measure hasn't happened yet
	int64_t mDelay;

	// what time we sent the delay meas
	openpal::UTCTimestamp mStart;
};

} //ens ns

#endif

