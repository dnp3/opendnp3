
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
#ifndef __APDU_CONSTANTS_H_
#define __APDU_CONSTANTS_H_

#include <string>

#include <opendnp3/gen/FunctionCode.h>
#include <opendnp3/gen/QualifierCode.h>

namespace opendnp3
{

/// Masks for the various parts of the control field
enum AppControlMasks {
	ACM_FIR = 0x80,
	ACM_FIN = 0x40,
	ACM_CON = 0x20,
	ACM_UNS = 0x10,
	ACM_SEQ = 0x0F
};

/// Indices for the various IIN bits
enum IINIndices {
	IINI_ALL_STATIONS = 0,
	IINI_CLASS1_EVENTS,
	IINI_CLASS2_EVENTS,
	IINI_CLASS3_EVENTS,
	IINI_NEED_TIME,
	IINI_LOCAL_CONTROL,
	IINI_DEVICE_TROUBLE,
	IINI_DEVICE_RESTART,
	IINI_FUNC_NOT_SUPPORTED,
	IINI_OBJECT_UNKNOWN,
	IINI_PARAM_ERROR,
	IINI_EVENT_BUFFER_OVERFLOW,
	IINI_ALREADY_EXECUTING,
	IINI_CONFIG_CORRUPT,
	IINI_RESERVED1,
	IINI_RESERVED2 = 15
};

/// Masks for the IIN least significant bit
enum IINMasksLSB {
	IIN_LSB_ALL_STATIONS = 0x01,
	IIN_LSB_CLASS1_EVENTS = 0x02,
	IIN_LSB_CLASS2_EVENTS = 0x04,
	IIN_LSB_CLASS3_EVENTS = 0x08,
	IIN_LSB_NEED_TIME = 0x10,
	IIN_LSB_LOCAL_CONTROL = 0x20,
	IIN_LSB_DEVICE_TROUBLE = 0x40,
	IIN_LSB_DEVICE_RESTART = 0x80
};

/// Masks for the IIN most significant bit
enum IINMasksMSB {
	IIN_MSB_FUNC_NOT_SUPPORTED = 0x01,
	IIN_MSB_OBJECT_UNKNOWN = 0x02,
	IIN_MSB_PARAM_ERROR = 0x04,
	IIN_MSB_EVENT_BUFFER_OVERFLOW = 0x08,
	IIN_MSB_ALREADY_EXECUTING = 0x10,
	IIN_MSB_CONFIG_CORRUPT = 0x20,
	IIN_MSB_RESERVED1 = 0x40,
	IIN_MSB_RESERVED2 = 0x80
};

/// @return true if the function is a response, false otherwise
bool IsResponse(FunctionCode aCode);

/// @return true if the function is a request, false otherwise
bool IsRequest(FunctionCode aCode);

}

#endif
