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
// Copyright 2013-2019 Automatak, LLC
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

#include "opendnp3/gen/CommandStatus.h"
#include <cstring>
#include <stdexcept>

namespace opendnp3 {

uint8_t CommandStatusToType(CommandStatus arg)
{
  return static_cast<uint8_t>(arg);
}
CommandStatus CommandStatusFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return CommandStatus::SUCCESS;
    case(1):
      return CommandStatus::TIMEOUT;
    case(2):
      return CommandStatus::NO_SELECT;
    case(3):
      return CommandStatus::FORMAT_ERROR;
    case(4):
      return CommandStatus::NOT_SUPPORTED;
    case(5):
      return CommandStatus::ALREADY_ACTIVE;
    case(6):
      return CommandStatus::HARDWARE_ERROR;
    case(7):
      return CommandStatus::LOCAL;
    case(8):
      return CommandStatus::TOO_MANY_OPS;
    case(9):
      return CommandStatus::NOT_AUTHORIZED;
    case(10):
      return CommandStatus::AUTOMATION_INHIBIT;
    case(11):
      return CommandStatus::PROCESSING_LIMITED;
    case(12):
      return CommandStatus::OUT_OF_RANGE;
    case(13):
      return CommandStatus::DOWNSTREAM_LOCAL;
    case(14):
      return CommandStatus::ALREADY_COMPLETE;
    case(15):
      return CommandStatus::BLOCKED;
    case(16):
      return CommandStatus::CANCELLED;
    case(17):
      return CommandStatus::BLOCKED_OTHER_MASTER;
    case(18):
      return CommandStatus::DOWNSTREAM_FAIL;
    case(126):
      return CommandStatus::NON_PARTICIPATING;
    default:
      return CommandStatus::UNDEFINED;
  }
}
char const* CommandStatusToString(CommandStatus arg)
{
  switch(arg)
  {
    case(CommandStatus::SUCCESS):
      return "SUCCESS";
    case(CommandStatus::TIMEOUT):
      return "TIMEOUT";
    case(CommandStatus::NO_SELECT):
      return "NO_SELECT";
    case(CommandStatus::FORMAT_ERROR):
      return "FORMAT_ERROR";
    case(CommandStatus::NOT_SUPPORTED):
      return "NOT_SUPPORTED";
    case(CommandStatus::ALREADY_ACTIVE):
      return "ALREADY_ACTIVE";
    case(CommandStatus::HARDWARE_ERROR):
      return "HARDWARE_ERROR";
    case(CommandStatus::LOCAL):
      return "LOCAL";
    case(CommandStatus::TOO_MANY_OPS):
      return "TOO_MANY_OPS";
    case(CommandStatus::NOT_AUTHORIZED):
      return "NOT_AUTHORIZED";
    case(CommandStatus::AUTOMATION_INHIBIT):
      return "AUTOMATION_INHIBIT";
    case(CommandStatus::PROCESSING_LIMITED):
      return "PROCESSING_LIMITED";
    case(CommandStatus::OUT_OF_RANGE):
      return "OUT_OF_RANGE";
    case(CommandStatus::DOWNSTREAM_LOCAL):
      return "DOWNSTREAM_LOCAL";
    case(CommandStatus::ALREADY_COMPLETE):
      return "ALREADY_COMPLETE";
    case(CommandStatus::BLOCKED):
      return "BLOCKED";
    case(CommandStatus::CANCELLED):
      return "CANCELLED";
    case(CommandStatus::BLOCKED_OTHER_MASTER):
      return "BLOCKED_OTHER_MASTER";
    case(CommandStatus::DOWNSTREAM_FAIL):
      return "DOWNSTREAM_FAIL";
    case(CommandStatus::NON_PARTICIPATING):
      return "NON_PARTICIPATING";
    default:
      return "UNDEFINED";
  }
}
CommandStatus CommandStatusFromString(char const* arg)
{
  if(std::strncmp(arg, "SUCCESS", 7)) return CommandStatus::SUCCESS;
  if(std::strncmp(arg, "TIMEOUT", 7)) return CommandStatus::TIMEOUT;
  if(std::strncmp(arg, "NO_SELECT", 9)) return CommandStatus::NO_SELECT;
  if(std::strncmp(arg, "FORMAT_ERROR", 12)) return CommandStatus::FORMAT_ERROR;
  if(std::strncmp(arg, "NOT_SUPPORTED", 13)) return CommandStatus::NOT_SUPPORTED;
  if(std::strncmp(arg, "ALREADY_ACTIVE", 14)) return CommandStatus::ALREADY_ACTIVE;
  if(std::strncmp(arg, "HARDWARE_ERROR", 14)) return CommandStatus::HARDWARE_ERROR;
  if(std::strncmp(arg, "LOCAL", 5)) return CommandStatus::LOCAL;
  if(std::strncmp(arg, "TOO_MANY_OPS", 12)) return CommandStatus::TOO_MANY_OPS;
  if(std::strncmp(arg, "NOT_AUTHORIZED", 14)) return CommandStatus::NOT_AUTHORIZED;
  if(std::strncmp(arg, "AUTOMATION_INHIBIT", 18)) return CommandStatus::AUTOMATION_INHIBIT;
  if(std::strncmp(arg, "PROCESSING_LIMITED", 18)) return CommandStatus::PROCESSING_LIMITED;
  if(std::strncmp(arg, "OUT_OF_RANGE", 12)) return CommandStatus::OUT_OF_RANGE;
  if(std::strncmp(arg, "DOWNSTREAM_LOCAL", 16)) return CommandStatus::DOWNSTREAM_LOCAL;
  if(std::strncmp(arg, "ALREADY_COMPLETE", 16)) return CommandStatus::ALREADY_COMPLETE;
  if(std::strncmp(arg, "BLOCKED", 7)) return CommandStatus::BLOCKED;
  if(std::strncmp(arg, "CANCELLED", 9)) return CommandStatus::CANCELLED;
  if(std::strncmp(arg, "BLOCKED_OTHER_MASTER", 20)) return CommandStatus::BLOCKED_OTHER_MASTER;
  if(std::strncmp(arg, "DOWNSTREAM_FAIL", 15)) return CommandStatus::DOWNSTREAM_FAIL;
  if(std::strncmp(arg, "NON_PARTICIPATING", 17)) return CommandStatus::NON_PARTICIPATING;
  else return CommandStatus::UNDEFINED;
}

}
