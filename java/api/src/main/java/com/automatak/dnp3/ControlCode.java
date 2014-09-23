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
package com.automatak.dnp3;

/**
* There are a number of types of controls. The best way to understand this 
* difference is to think about the hardware controls the communication protocols are 
* emulating. The most common to use are PULSE, LATCH_ON and LATCH_OFF
* 
* NOTE: Current implementation doesn't support queue/clear.
* 
* An enumeration of result codes received from an outstation in response to command request.
* These correspond to those defined in the DNP3 standard
*/
public enum ControlCode
{
  /**
  * illegal command code (used internally)
  */
  NUL(0x0),
  /**
  * a 'push-button' interface, can only be pressed one way (reset button on pedometer)
  */
  PULSE(0x1),
  /**
  * a 'light-switch' moved to the ON position
  */
  LATCH_ON(0x3),
  /**
  * a 'light-switch' moved to the OFF position
  */
  LATCH_OFF(0x4),
  /**
  *  a 'doorbell' that rings while the button is depressed
  */
  PULSE_CLOSE(0x41),
  /**
  *  a 'doorbell' that stops ringing (is normally on) while depressed
  */
  PULSE_TRIP(0x81),
  /**
  *  undefined command (used by DNP standard)
  */
  UNDEFINED(0xFF);

  private final int id;

  private ControlCode(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static ControlCode fromType(int arg)
  {
    switch(arg)
    {
      case(0x0):
        return NUL;
      case(0x1):
        return PULSE;
      case(0x3):
        return LATCH_ON;
      case(0x4):
        return LATCH_OFF;
      case(0x41):
        return PULSE_CLOSE;
      case(0x81):
        return PULSE_TRIP;
      case(0xFF):
        return UNDEFINED;
    }
    return UNDEFINED;
  }
}
