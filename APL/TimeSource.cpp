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
#include "TimeSource.h"

using namespace std::chrono;

namespace apl
{

//real time source

TimeSource TimeSource::mInstance;

high_resolution_clock::time_point TimeSource::GetUTC()
{
	return high_resolution_clock::now();
}

//mock time source
MockTimeSource::MockTimeSource()	
{

}

void MockTimeSource::Advance(const std::chrono::high_resolution_clock::duration& arDuration)
{
	mTime += arDuration;
}

void MockTimeSource::SetToNow()
{
	mTime = high_resolution_clock::now();
}


TimeSourceSystemOffset::TimeSourceSystemOffset()
	: mOffset(0)
{
}


std::chrono::high_resolution_clock::time_point TimeSourceSystemOffset::GetUTC()
{
	return high_resolution_clock::now() + mOffset;
}

void TimeSourceSystemOffset::SetTime(const std::chrono::high_resolution_clock::time_point& arTime)
{
	mOffset = arTime - high_resolution_clock::now();
}

}
