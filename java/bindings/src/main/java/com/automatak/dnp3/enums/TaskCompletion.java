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

package com.automatak.dnp3.enums;
/**
* Enum that describes if a master task succeeded or failed
*/
public enum TaskCompletion
{
  /**
  * A valid response was received from the outstation
  */
  SUCCESS(0),
  /**
  * A response was received from the outstation, but it was not valid
  */
  FAILURE_BAD_RESPONSE(1),
  /**
  * The task request did not receive a response within the timeout
  */
  FAILURE_RESPONSE_TIMEOUT(2),
  /**
  * The start timeout expired before the task could begin running
  */
  FAILURE_START_TIMEOUT(3),
  /**
  * The task failed because the master was unable to format the request
  */
  FAILURE_MESSAGE_FORMAT_ERROR(4),
  /**
  * There is no communication with the outstation, so the task was not attempted
  */
  FAILURE_NO_COMMS(255);

  private final int id;

  public int toType()
  {
    return id;
  }

  TaskCompletion(int id)
  {
    this.id = id;
  }

  public static TaskCompletion fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return SUCCESS;
      case(1):
        return FAILURE_BAD_RESPONSE;
      case(2):
        return FAILURE_RESPONSE_TIMEOUT;
      case(3):
        return FAILURE_START_TIMEOUT;
      case(4):
        return FAILURE_MESSAGE_FORMAT_ERROR;
      default:
        return FAILURE_NO_COMMS;
    }
  }
}
