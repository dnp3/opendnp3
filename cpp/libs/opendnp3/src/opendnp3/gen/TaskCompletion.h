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

#ifndef OPENDNP3_TASKCOMPLETION_H
#define OPENDNP3_TASKCOMPLETION_H

#include <cstdint>

namespace opendnp3 {

/**
  Enum that describes if a master task succeeded or failed
*/
enum class TaskCompletion : int
{
  /// A valid response was received from the outstation
  SUCCESS = 0,
  /// A response was received from the outstation, but it was not valid
  FAILURE_BAD_RESPONSE = 1,
  /// The task request did not receive a response within the timeout
  FAILURE_RESPONSE_TIMEOUT = 2,
  /// There is no communication with the outstation, so the task was not attempted
  FAILURE_NO_COMMS = 3
};

char const* TaskCompletionToString(TaskCompletion arg);

}

#endif
