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
#include "DeviceTemplate.h"

namespace apl
{
namespace dnp
{

DeviceTemplate::DeviceTemplate(size_t aNumBinary,
                               size_t aNumAnalog,
                               size_t aNumCounter,
                               size_t aNumControlStatus,
                               size_t aNumSetpointStatus,
                               size_t aNumControls,
                               size_t aNumSetpoints) :
	mStartOnline(false)
{
	this->mBinary.resize(aNumBinary);	this->InitNames("Binary", mBinary);
	this->mAnalog.resize(aNumAnalog);	this->InitNames("Analog", mAnalog);
	this->mCounter.resize(aNumCounter); this->InitNames("Counter", mCounter);
	this->mControlStatus.resize(aNumControlStatus); this->InitNames("ControlStatus", mControlStatus);
	this->mSetpointStatus.resize(aNumSetpointStatus); this->InitNames("SetpointStatus", mSetpointStatus);
	this->mControls.resize(aNumControls); this->InitNames("Control", mControls);
	this->mSetpoints.resize(aNumSetpoints); this->InitNames("Setpoint", mSetpoints);
}

void DeviceTemplate::Publish(IDataObserver* apObs)
{
	Transaction tr(apObs);
	InitObserver<Binary>(apObs, mBinary.size());
	InitObserver<Analog>(apObs, mAnalog.size());
	InitObserver<Counter>(apObs, mCounter.size());
	InitObserver<ControlStatus>(apObs, mControlStatus.size());
	InitObserver<SetpointStatus>(apObs, mSetpointStatus.size());
}

}
}


