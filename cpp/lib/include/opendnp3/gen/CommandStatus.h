//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013-2020 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3_COMMANDSTATUS_H
#define OPENDNP3_COMMANDSTATUS_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  An enumeration of result codes received from an outstation in response to command request.
  These correspond to those defined in the DNP3 standard
*/
enum class CommandStatus : uint8_t
{
  /// command was accepted, initiated, or queued
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
  /// command not accepted because it was prevented or inhibited by a local automation process, such as interlocking logic or synchrocheck
  AUTOMATION_INHIBIT = 10,
  /// command not accepted because the device cannot process any more activities than are presently in progress
  PROCESSING_LIMITED = 11,
  /// command not accepted because the value is outside the acceptable range permitted for this point
  OUT_OF_RANGE = 12,
  /// command not accepted because the outstation is forwarding the request to another downstream device which reported LOCAL
  DOWNSTREAM_LOCAL = 13,
  /// command not accepted because the outstation has already completed the requested operation
  ALREADY_COMPLETE = 14,
  /// command not accepted because the requested function is specifically blocked at the outstation
  BLOCKED = 15,
  /// command not accepted because the operation was cancelled
  CANCELLED = 16,
  /// command not accepted because another master is communicating with the outstation and has exclusive rights to operate this control point
  BLOCKED_OTHER_MASTER = 17,
  /// command not accepted because the outstation is forwarding the request to another downstream device which cannot be reached or is otherwise incapable of performing the request
  DOWNSTREAM_FAIL = 18,
  /// (deprecated) indicates the outstation shall not issue or perform the control operation
  NON_PARTICIPATING = 126,
  /// 10 to 126 are currently reserved
  UNDEFINED = 127
};

struct CommandStatusSpec
{
  using enum_type_t = CommandStatus;

  static uint8_t to_type(CommandStatus arg);
  static CommandStatus from_type(uint8_t arg);
  static char const* to_string(CommandStatus arg);
  static char const* to_human_string(CommandStatus arg);
  static CommandStatus from_string(const std::string& arg);
};

}

#endif
