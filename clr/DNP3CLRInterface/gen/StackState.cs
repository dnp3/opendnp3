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
  /// Enumeration for possible communication states of a stack
  /// </summary>
  public enum StackState : int
  {
    /// <summary>
    /// communications are online
    /// </summary>
    COMMS_UP = 0,
    /// <summary>
    /// communication difficulties
    /// </summary>
    COMMS_DOWN = 1,
    /// <summary>
    /// unknown state
    /// </summary>
    UNKNOWN = 2
  }
}
