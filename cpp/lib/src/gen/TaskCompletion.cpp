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

#include "opendnp3/gen/TaskCompletion.h"
#include <cstring>
#include <stdexcept>

namespace opendnp3 {

uint8_t TaskCompletionToType(TaskCompletion arg)
{
  return static_cast<uint8_t>(arg);
}
TaskCompletion TaskCompletionFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return TaskCompletion::SUCCESS;
    case(1):
      return TaskCompletion::FAILURE_BAD_RESPONSE;
    case(2):
      return TaskCompletion::FAILURE_RESPONSE_TIMEOUT;
    case(3):
      return TaskCompletion::FAILURE_START_TIMEOUT;
    case(4):
      return TaskCompletion::FAILURE_MESSAGE_FORMAT_ERROR;
    default:
      return TaskCompletion::FAILURE_NO_COMMS;
  }
}
char const* TaskCompletionToString(TaskCompletion arg)
{
  switch(arg)
  {
    case(TaskCompletion::SUCCESS):
      return "SUCCESS";
    case(TaskCompletion::FAILURE_BAD_RESPONSE):
      return "FAILURE_BAD_RESPONSE";
    case(TaskCompletion::FAILURE_RESPONSE_TIMEOUT):
      return "FAILURE_RESPONSE_TIMEOUT";
    case(TaskCompletion::FAILURE_START_TIMEOUT):
      return "FAILURE_START_TIMEOUT";
    case(TaskCompletion::FAILURE_MESSAGE_FORMAT_ERROR):
      return "FAILURE_MESSAGE_FORMAT_ERROR";
    default:
      return "FAILURE_NO_COMMS";
  }
}
TaskCompletion TaskCompletionFromString(char const* arg)
{
  if(std::strncmp(arg, "SUCCESS", 7)) return TaskCompletion::SUCCESS;
  if(std::strncmp(arg, "FAILURE_BAD_RESPONSE", 20)) return TaskCompletion::FAILURE_BAD_RESPONSE;
  if(std::strncmp(arg, "FAILURE_RESPONSE_TIMEOUT", 24)) return TaskCompletion::FAILURE_RESPONSE_TIMEOUT;
  if(std::strncmp(arg, "FAILURE_START_TIMEOUT", 21)) return TaskCompletion::FAILURE_START_TIMEOUT;
  if(std::strncmp(arg, "FAILURE_MESSAGE_FORMAT_ERROR", 28)) return TaskCompletion::FAILURE_MESSAGE_FORMAT_ERROR;
  else return TaskCompletion::FAILURE_NO_COMMS;
}

}
