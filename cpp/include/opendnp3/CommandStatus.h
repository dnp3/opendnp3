
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
#ifndef __COMMAND_STATUS_H_
#define __COMMAND_STATUS_H_

#include <cstdint>
#include <string>

namespace opendnp3
{

/**
 * When a command is recieved from a master the application sends a code to
 * indicate if it was successfull or if not what class of error was
 * encountered.  Each code has a description that indicates its customary
 * meaning.
 */
enum CommandStatus {
	CS_SUCCESS = 0,			//!< command was successfully recieved and handled
	CS_TIMEOUT = 1,			//!< command timed out before completing
	CS_NO_SELECT = 2,		//!< command requires being selected before operate, configuration issue
	CS_FORMAT_ERROR = 3,	//!< bad control code or timing values
	CS_NOT_SUPPORTED = 4,	//!< command is not implemented
	CS_ALREADY_ACTIVE = 5,	//!< command is allready in progress or its allready in that mode
	CS_HARDWARE_ERROR = 6,	//!< something is stopping the command, often a local/remote interlock
	CS_LOCAL = 7,			//!< the function goverened by the control is in local only control
	CS_TOO_MANY_OPS = 8,	//!< the command has been done too often and has been throttled
	CS_NOT_AUTHORIZED = 9,	//!< the command was rejected because the device denied it or an RTU intercepted it
	CS_UNDEFINED = 127		//!< 10 to 126 are currently reserved
};

/**
* Converts an integer to a CommandStatus enumeration
*
* @param aField The integer to convert
* @return CommandStatus enumeration for the integer value
*/
CommandStatus IntToCommandStatus(int aField);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
/**
* Converts a CommandStatus enumeration to string representation
*
* @param aCode The status to convert
* @return string representation of the command status
*/
std::string ToString(CommandStatus aCode);
#endif

}

/* vim: set ts=4 sw=4: */

#endif
