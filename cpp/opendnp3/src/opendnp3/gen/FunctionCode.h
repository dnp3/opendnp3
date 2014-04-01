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

#ifndef __OPENDNP3_GENERATED_FUNCTIONCODE_H_
#define __OPENDNP3_GENERATED_FUNCTIONCODE_H_

#include <string>
#include <cstdint>

namespace opendnp3 {

/**
  Application layer function code enumeration
*/
enum class FunctionCode : uint8_t
{
  CONFIRM = 0x0,
  READ = 0x1,
  WRITE = 0x2,
  SELECT = 0x3,
  OPERATE = 0x4,
  DIRECT_OPERATE = 0x5,
  DIRECT_OPERATE_NO_ACK = 0x6,
  FREEZE = 0x7,
  FREEZE_NO_ACK = 0x8,
  FREEZE_CLEAR = 0x9,
  FREEZE_CLEAR_NO_ACK = 0xA,
  FREEZE_AT_TIME = 0xB,
  FREEZE_AT_TIME_NO_ACK = 0xC,
  COLD_RESTART = 0xD,
  WARM_RESTART = 0xE,
  INITIALIZE_DATA = 0xF,
  INITIALIZE_APPLICATION = 0x10,
  START_APPLICATION = 0x11,
  STOP_APPLICATION = 0x12,
  SAVE_CONFIGURATION = 0x13,
  ENABLE_UNSOLICITED = 0x14,
  DISABLE_UNSOLICITED = 0x15,
  ASSIGN_CLASS = 0x16,
  DELAY_MEASURE = 0x17,
  RECORD_TIME = 0x18,
  FILE_OPEN = 0x19,
  FILE_CLOSE = 0x1A,
  FILE_DELETE = 0x1B,
  FILE_INFO = 0x1C,
  FILE_AUTHENTICATE = 0x1D,
  FILE_ABORT = 0x1E,
  RESPONSE = 0x81,
  UNSOLICITED_RESPONSE = 0x82,
  UNKNOWN = 0xFF
};

uint8_t FunctionCodeToType(FunctionCode arg);
FunctionCode FunctionCodeFromType(uint8_t arg);
std::string FunctionCodeToString(FunctionCode arg);

}

#endif
