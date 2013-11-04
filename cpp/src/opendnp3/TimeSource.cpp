
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

#include "TimeSource.h"

using namespace std::chrono;

namespace opendnp3
{

//real time source

TimeSource TimeSource::mInstance;

std::chrono::system_clock::time_point TimeSource::GetUTC()
{
	return std::chrono::system_clock::now();
}

TimeSourceSystemOffset::TimeSourceSystemOffset()
	: mOffset(0)
{
}

std::chrono::system_clock::time_point TimeSourceSystemOffset::GetUTC()
{
	return std::chrono::system_clock::now() + mOffset;
}

void TimeSourceSystemOffset::SetTime(const std::chrono::system_clock::time_point& arTime)
{
	mOffset = arTime - std::chrono::system_clock::now();
}

}
