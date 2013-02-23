
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
#include <opendnp3/BaseDataTypes.h>

using namespace std;

namespace opendnp3
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string GetDataTypeName(DataTypes aType)
{
	switch(aType) {
	case(DT_BINARY):
		return "Binary";
	case(DT_ANALOG):
		return "Analog";
	case(DT_COUNTER):
		return "Counter";
	case(DT_CONTROL_STATUS):
		return "ControlStatus";
	case(DT_SETPOINT_STATUS):
		return "SetpointStatus";
	default:
		return "Unknown";
	}
}
#endif


// DataPoint


DataPoint::DataPoint(uint8_t aQuality, DataTypes aType) :
	mQuality(aQuality), mTime(0),
	mType(aType)
{}

// BoolDataPoint


BoolDataPoint::BoolDataPoint(uint8_t aQuality, DataTypes aType, uint8_t aValueMask) :
	DataPoint(aQuality, aType),
	mValueMask(aValueMask)
{}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string BoolDataPoint::ToString() const
{
	std::ostringstream oss;
	oss << "Value: " << (GetValue() ? "true" : "false") << " Quality: " << static_cast<int>(GetQuality());
	return oss.str();
}
#endif

template<>
bool ExceedsDeadband<double>(const double& val1, const double& val2, double aDeadband)
{
	double val = fabs(val1 - val2);

	if(val == std::numeric_limits<double>::infinity()) return true;
	else {
		return fabs(val) > aDeadband;
	}

}
}

