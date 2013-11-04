
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

#ifndef __I_TIME_SOURCE_H_
#define __I_TIME_SOURCE_H_

#include <opendnp3/Types.h>
#include <opendnp3/Visibility.h>
#include <opendnp3/Clock.h>

#include <chrono>

namespace opendnp3
{

class DLL_LOCAL IMonotonicTimeSource
{

public:
	virtual timer_clock::time_point Now() = 0;

};

class DLL_LOCAL ITimeSource
{

public:
	virtual std::chrono::system_clock::time_point GetUTC() = 0;
	millis_t GetMillisecondsSinceEpoch();

};

class DLL_LOCAL ITimeManager : public ITimeSource
{
public:
	virtual void SetTime(const std::chrono::system_clock::time_point& arTime) = 0;
};



}

#endif

