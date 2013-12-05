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
//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef __OPENDNP3_GENERATED_COMMANDSTATUS_H_
#define __OPENDNP3_GENERATED_COMMANDSTATUS_H_

#include <string>
#include <cstdint>

namespace opendnp3 {

/**
  An enumeration of result codes received from an outstation in response to command request.
  These correspond to those defined in the DNP3 standard
*/
enum class CommandStatus : uint8_t
{
  /// command was successfully received and handled
  SUCCESS = 0,
  /// command timed out before completing
  TIMEOUT = 1,
  /// command requires being selected before operate, configuration issue
  NO_SELECT = 2,
  /// bad control code or timing values
  FORMAT_ERROR = 3,
  /// command is not implemented
  NOT_SUPPORTED = 4,
  /// command is all ready in progress or its all ready in that mode
  ALREADY_ACTIVE = 5,
  /// something is stopping the command, often a local/remote interlock
  HARDWARE_ERROR = 6,
  /// the function governed by the control is in local only control
  LOCAL = 7,
  /// the command has been done too often and has been throttled
  TOO_MANY_OPS = 8,
  /// the command was rejected because the device denied it or an RTU intercepted it
  NOT_AUTHORIZED = 9,
  /// 10 to 126 are currently reserved
  UNDEFINED = 127
};

std::string CommandStatusToString(CommandStatus arg);
uint8_t CommandStatusToType(CommandStatus arg);
CommandStatus CommandStatusFromType(uint8_t arg);

}

#endif
