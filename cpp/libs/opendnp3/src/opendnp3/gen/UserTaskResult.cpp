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

#include "UserTaskResult.h"

namespace opendnp3 {

char const* UserTaskResultToString(UserTaskResult arg)
{
  switch(arg)
  {
    case(UserTaskResult::RESPONSE_OK):
      return "RESPONSE_OK";
    case(UserTaskResult::BAD_RESPONSE):
      return "BAD_RESPONSE";
    case(UserTaskResult::TIMEOUT):
      return "TIMEOUT";
    case(UserTaskResult::NO_COMMS):
      return "NO_COMMS";
  }
  return "NO_COMMS";
}

}
