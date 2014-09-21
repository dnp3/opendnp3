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

#ifndef OPENDNP3_CONTROLCODE_H
#define OPENDNP3_CONTROLCODE_H

#include <cstdint>

namespace opendnp3 {

/**
  There are a number of types of controls. The best way to understand this 
  difference is to think about the hardware controls the communication protocols are 
  emulating. The most common to use are PULSE, LATCH_ON and LATCH_OFF

  NOTE: Current implementation doesn't support queue/clear.

  An enumeration of result codes received from an outstation in response to command request.
  These correspond to those defined in the DNP3 standard
*/
enum class ControlCode : uint8_t
{
  /// illegal command code (used internally)
  NUL = 0x0,
  /// a 'push-button' interface, can only be pressed one way (reset button on pedometer)
  PULSE = 0x1,
  /// a 'light-switch' moved to the ON position
  LATCH_ON = 0x3,
  /// a 'light-switch' moved to the OFF position
  LATCH_OFF = 0x4,
  ///  a 'doorbell' that rings while the button is depressed
  PULSE_CLOSE = 0x41,
  ///  a 'doorbell' that stops ringing (is normally on) while depressed
  PULSE_TRIP = 0x81,
  ///  undefined command (used by DNP standard)
  UNDEFINED = 0xFF
};

uint8_t ControlCodeToType(ControlCode arg);
ControlCode ControlCodeFromType(uint8_t arg);
char const* ControlCodeToString(ControlCode arg);

}

#endif
