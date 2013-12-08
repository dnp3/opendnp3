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
#ifndef __MEASUREMENT_HELPERS_H_
#define	__MEASUREMENT_HELPERS_H_

#include <opendnp3/BaseDataTypes.h>
#include <opendnp3/DataTypes.h>
#include <openpal/ToHex.h>

#include <sstream>

namespace opendnp3
{
	
template <class T>
std::string ToString(const TypedMeasurement<T>& arMeas)
{
	std::ostringstream oss;
	oss << "Value: " << arMeas.GetValue() << " Quality: 0x" << openpal::ByteToHex(arMeas.GetQuality()) << " Time: " << arMeas.GetTime();
	return oss.str();
}

template <class T>
bool ExceedsDeadband(const T& val1, const T& val2, double aDeadband)
{
	// T can be unsigned data type so std::abs won't work since it only directly supports signed data types
	// If one uses std::abs and T is unsigned one will get an ambiguous override error.
	auto diff = (val1 < val2) ? (val2 - val1) : (val1 - val2);

	return (diff > aDeadband);
}

template <>
bool ExceedsDeadband<double>(const double& val1, const double& val2, double aDeadband);

template <class T>
bool IsChangeEvent(const T& newValue, const T& aLastReport, double aDeadband)
{
	if(newValue.GetQuality() != aLastReport.GetQuality()) return true;
	else {
		return ExceedsDeadband<typename T::Type>(newValue.GetValue(), aLastReport.GetValue(), aDeadband);
	}            
}

//specialization for BoolDataPoint
template <>
bool IsChangeEvent<Binary>(const Binary& newValue, const Binary& aLastReport, double aDeadband);

}

#endif

