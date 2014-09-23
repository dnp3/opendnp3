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
* An enumeration of result codes received from an outstation in response to command request.
* These correspond to those defined in the DNP3 standard
*/
public enum CommandStatus
{
  /**
  * command was successfully received and handled
  */
  SUCCESS(0),
  /**
  * command timed out before completing
  */
  TIMEOUT(1),
  /**
  * command requires being selected before operate, configuration issue
  */
  NO_SELECT(2),
  /**
  * bad control code or timing values
  */
  FORMAT_ERROR(3),
  /**
  * command is not implemented
  */
  NOT_SUPPORTED(4),
  /**
  * command is all ready in progress or its all ready in that mode
  */
  ALREADY_ACTIVE(5),
  /**
  * something is stopping the command, often a local/remote interlock
  */
  HARDWARE_ERROR(6),
  /**
  * the function governed by the control is in local only control
  */
  LOCAL(7),
  /**
  * the command has been done too often and has been throttled
  */
  TOO_MANY_OPS(8),
  /**
  * the command was rejected because the device denied it or an RTU intercepted it
  */
  NOT_AUTHORIZED(9),
  /**
  * 10 to 126 are currently reserved
  */
  UNDEFINED(127);

  private final int id;

  private CommandStatus(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static CommandStatus fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return SUCCESS;
      case(1):
        return TIMEOUT;
      case(2):
        return NO_SELECT;
      case(3):
        return FORMAT_ERROR;
      case(4):
        return NOT_SUPPORTED;
      case(5):
        return ALREADY_ACTIVE;
      case(6):
        return HARDWARE_ERROR;
      case(7):
        return LOCAL;
      case(8):
        return TOO_MANY_OPS;
      case(9):
        return NOT_AUTHORIZED;
      case(127):
        return UNDEFINED;
    }
    return UNDEFINED;
  }
}
