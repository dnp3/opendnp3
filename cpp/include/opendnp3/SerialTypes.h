
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
#ifndef __SERIAL_TYPES_H_
#define __SERIAL_TYPES_H_

#include <string>

namespace opendnp3
{

/// Enumeration for setting serial port parity 
enum ParityType {
	PAR_NONE = 0,
	PAR_EVEN = 1,
	PAR_ODD = 2
};

/// Enumeration for setting serial port flow control
enum FlowType {
	FLOW_NONE = 0,
	FLOW_HARDWARE = 1,
	FLOW_XONXOFF = 2
};

ParityType GetParityFromInt(int parity);
FlowType GetFlowTypeFromInt(int parity);

/// Settings structure for the serial port
struct SerialSettings {
	/// name of the port, i.e. "COM1" or "/dev/tty0"
	std::string mDevice;
	/// Baud rate of the port, i.e. 9600 or 57600
	int mBaud;
	/// Data bits, usually 8
	int mDataBits;
	/// Stop bits, usually set to 1
	int mStopBits;
	/// Parity setting for the port, usually PAR_NONE
	ParityType mParity;
	/// Flow control setting, usually FLOW_NONE
	FlowType mFlowType;
};

}

#endif

