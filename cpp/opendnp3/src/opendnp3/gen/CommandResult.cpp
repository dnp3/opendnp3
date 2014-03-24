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

#include "CommandResult.h"

namespace opendnp3 {

std::string CommandResultToString(CommandResult arg)
{
  switch(arg)
  {
    case(CommandResult::RESPONSE_OK):
      return "RESPONSE_OK";
    case(CommandResult::BAD_RESPONSE):
      return "BAD_RESPONSE";
    case(CommandResult::TIMEOUT):
      return "TIMEOUT";
    case(CommandResult::NO_COMMS):
      return "NO_COMMS";
  }
  return "NO_COMMS";
}

}
