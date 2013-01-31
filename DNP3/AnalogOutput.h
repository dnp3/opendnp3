
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
#ifndef __ANALOG_OUTPUT_H_
#define __ANALOG_OUTPUT_H_

#include "CommandStatus.h"

namespace apl
{
namespace dnp
{

/**
 * The object to represent a setpoint request from the master. Think of
 * this like turning a dial on the front of a machine to desired setting.
 * 
 */
template <class T>
class AnalogOutput
{
public:

	/**
	 * Creates a new instance with underlying type T
     */
	AnalogOutput(T aValue) : 
		mValue(aValue),
		mStatus(CommandStatus::CS_SUCCESS)
	{}

	virtual std::string ToString() const = 0;

	T GetValue() const {
		return mValue;
	}		

public:
	CommandStatus mStatus;
	
protected:
	T mValue;
};

class AnalogOutputInt16 : public AnalogOutput<int16_t>
{
	public:	

	AnalogOutputInt16(int16_t);

	bool operator==(const AnalogOutputInt16& arRHS) const;

	std::string ToString() const;
};

class AnalogOutputInt32 : public AnalogOutput<int32_t>
{	
	public:	

	AnalogOutputInt32(int32_t);

	bool operator==(const AnalogOutputInt32& arRHS) const;

	std::string ToString() const;
};

class AnalogOutputFloat32 : public AnalogOutput<float>
{	
	public:	

	AnalogOutputFloat32(float);

	bool operator==(const AnalogOutputFloat32& arRHS) const;

	std::string ToString() const;
};

class AnalogOutputDouble64 : public AnalogOutput<double>
{	
	public:

	AnalogOutputDouble64(double);

	bool operator==(const AnalogOutputDouble64& arRHS) const;

	std::string ToString() const;
};


}}

/* vim: set ts=4 sw=4: */

#endif
