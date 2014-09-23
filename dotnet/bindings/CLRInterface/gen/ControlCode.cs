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
  /// There are a number of types of controls. The best way to understand this 
  /// difference is to think about the hardware controls the communication protocols are 
  /// emulating. The most common to use are PULSE, LATCH_ON and LATCH_OFF
  /// 
  /// NOTE: Current implementation doesn't support queue/clear.
  /// 
  /// An enumeration of result codes received from an outstation in response to command request.
  /// These correspond to those defined in the DNP3 standard
  /// </summary>
  public enum ControlCode : byte
  {
    /// <summary>
    /// illegal command code (used internally)
    /// </summary>
    NUL = 0x0,
    /// <summary>
    /// a 'push-button' interface, can only be pressed one way (reset button on pedometer)
    /// </summary>
    PULSE = 0x1,
    /// <summary>
    /// a 'light-switch' moved to the ON position
    /// </summary>
    LATCH_ON = 0x3,
    /// <summary>
    /// a 'light-switch' moved to the OFF position
    /// </summary>
    LATCH_OFF = 0x4,
    /// <summary>
    ///  a 'doorbell' that rings while the button is depressed
    /// </summary>
    PULSE_CLOSE = 0x41,
    /// <summary>
    ///  a 'doorbell' that stops ringing (is normally on) while depressed
    /// </summary>
    PULSE_TRIP = 0x81,
    /// <summary>
    ///  undefined command (used by DNP standard)
    /// </summary>
    UNDEFINED = 0xFF
  }
}
