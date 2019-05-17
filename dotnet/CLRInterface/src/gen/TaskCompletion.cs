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

namespace Automatak.DNP3.Interface
{
  /// <summary>
  /// Enum that describes if a master task succeeded or failed
  /// </summary>
  public enum TaskCompletion : byte
  {
    /// <summary>
    /// A valid response was received from the outstation
    /// </summary>
    SUCCESS = 0,
    /// <summary>
    /// A response was received from the outstation, but it was not valid
    /// </summary>
    FAILURE_BAD_RESPONSE = 1,
    /// <summary>
    /// The task request did not receive a response within the timeout
    /// </summary>
    FAILURE_RESPONSE_TIMEOUT = 2,
    /// <summary>
    /// The start timeout expired before the task could begin running
    /// </summary>
    FAILURE_START_TIMEOUT = 3,
    /// <summary>
    /// The task failed because the master was unable to format the request
    /// </summary>
    FAILURE_MESSAGE_FORMAT_ERROR = 4,
    /// <summary>
    /// There is no communication with the outstation, so the task was not attempted
    /// </summary>
    FAILURE_NO_COMMS = 255
  }
}
