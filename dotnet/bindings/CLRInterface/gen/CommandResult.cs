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
  /// Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation
  /// </summary>
  public enum CommandResult : int
  {
    /// <summary>
    /// A response was received from the outstation, check the CommandStatus enumeration
    /// </summary>
    RESPONSE_OK = 0,
    /// <summary>
    /// A response was received from the outstation, but it did not match or contained bad formatting
    /// </summary>
    BAD_RESPONSE = 1,
    /// <summary>
    /// The operation timed out without a response
    /// </summary>
    TIMEOUT = 2,
    /// <summary>
    /// There is no communication with the outstation, and the command was not attempted
    /// </summary>
    NO_COMMS = 3,
    /// <summary>
    /// The master's requst queue is full. Too many operations have been requested
    /// </summary>
    QUEUE_FULL = 4
  }
}
