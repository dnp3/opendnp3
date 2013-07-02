
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

#include <opendnp3/AnalogOutput.h>

#include <math.h>
#include <sstream>

using namespace std;

namespace opendnp3
{

AnalogOutputInt16::AnalogOutputInt16(int16_t aValue) : AnalogOutput<int16_t>(aValue)
{}

AnalogOutputInt16::AnalogOutputInt16() : AnalogOutput<int16_t>(0)
{}

bool AnalogOutputInt16::operator==(const AnalogOutputInt16& arRHS) const
{
	return mValue == arRHS.mValue;
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string AnalogOutputInt16::ToString() const
{
	ostringstream oss;
	oss << "AnalogOutputInt16: " << mValue;
	return oss.str();
}
#endif

AnalogOutputInt32::AnalogOutputInt32(int32_t aValue) : AnalogOutput<int32_t>(aValue)
{}

AnalogOutputInt32::AnalogOutputInt32() : AnalogOutput<int32_t>(0)
{}

bool AnalogOutputInt32::operator==(const AnalogOutputInt32& arRHS) const
{
	return mValue == arRHS.mValue;
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string AnalogOutputInt32::ToString() const
{
	ostringstream oss;
	oss << "AnalogOutputInt32: " << mValue;
	return oss.str();
}
#endif

AnalogOutputFloat32::AnalogOutputFloat32(float aValue) : AnalogOutput<float>(aValue)
{}

AnalogOutputFloat32::AnalogOutputFloat32() : AnalogOutput<float>(0)
{}

bool AnalogOutputFloat32::operator==(const AnalogOutputFloat32& arRHS) const
{
	return mValue == arRHS.mValue;
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string AnalogOutputFloat32::ToString() const
{
	ostringstream oss;
	oss << "AnalogOutputFloat32: " << mValue;
	return oss.str();
}
#endif

AnalogOutputDouble64::AnalogOutputDouble64(double aValue) : AnalogOutput<double>(aValue)
{}

AnalogOutputDouble64::AnalogOutputDouble64() : AnalogOutput<double>(0)
{}

bool AnalogOutputDouble64::operator==(const AnalogOutputDouble64& arRHS) const
{
	return mValue == arRHS.mValue;
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string AnalogOutputDouble64::ToString() const
{
	ostringstream oss;
	oss << "AnalogOutputDouble64: " << mValue;
	return oss.str();
}
#endif

}

/* vim: set ts=4 sw=4: */
