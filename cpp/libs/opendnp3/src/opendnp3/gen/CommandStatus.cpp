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
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "CommandStatus.h"

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
    case(127):
      return CommandStatus::UNDEFINED;
  }
  return CommandStatus::UNDEFINED;
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
    case(CommandStatus::UNDEFINED):
      return "UNDEFINED";
  }
  return "UNDEFINED";
}

}
