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
* Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation
*/
public enum CommandResult
{
  /**
  * A response was received from the outstation, check the CommandStatus enumeration
  */
  RESPONSE_OK(0),
  /**
  * A response was received from the outstation, but it did not match or contained bad formatting
  */
  BAD_RESPONSE(1),
  /**
  * The operation timed out without a response
  */
  TIMEOUT(2),
  /**
  * There is no communication with the outstation, and the command was not attempted
  */
  NO_COMMS(3),
  /**
  * The master's requst queue is full. Too many operations have been requested
  */
  QUEUE_FULL(4);

  private final int id;

  private CommandResult(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static CommandResult fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return RESPONSE_OK;
      case(1):
        return BAD_RESPONSE;
      case(2):
        return TIMEOUT;
      case(3):
        return NO_COMMS;
      case(4):
        return QUEUE_FULL;
    }
    return QUEUE_FULL;
  }
}
