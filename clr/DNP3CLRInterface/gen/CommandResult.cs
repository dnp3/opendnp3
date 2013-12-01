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


namespace DNP3.Interface
{
  /// <summary>
  /// Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation
  /// </summary>
  public enum CommandResult : byte
  {
    /// <summary>
    /// A response was received from the outstation, check the CommandStatus enumeration
    /// </summary>
    RESPONSE_OK = 0,
    /// <summary>
    /// The operation timed out without a response
    /// </summary>
    TIMEOUT = 1,
    /// <summary>
    /// There is no communication with the outstation, and the command was not attempted
    /// </summary>
    NO_COMMS = 2
  }
}
