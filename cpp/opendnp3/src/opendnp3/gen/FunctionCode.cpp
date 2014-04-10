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

#include "FunctionCode.h"

namespace opendnp3 {

uint8_t FunctionCodeToType(FunctionCode arg)
{
  return static_cast<uint8_t>(arg);
}

FunctionCode FunctionCodeFromType(uint8_t arg)
{
  return static_cast<FunctionCode>(arg);
}

char const* FunctionCodeToString(FunctionCode arg)
{
  switch(arg)
  {
    case(FunctionCode::CONFIRM):
      return "CONFIRM";
    case(FunctionCode::READ):
      return "READ";
    case(FunctionCode::WRITE):
      return "WRITE";
    case(FunctionCode::SELECT):
      return "SELECT";
    case(FunctionCode::OPERATE):
      return "OPERATE";
    case(FunctionCode::DIRECT_OPERATE):
      return "DIRECT_OPERATE";
    case(FunctionCode::DIRECT_OPERATE_NO_ACK):
      return "DIRECT_OPERATE_NO_ACK";
    case(FunctionCode::FREEZE):
      return "FREEZE";
    case(FunctionCode::FREEZE_NO_ACK):
      return "FREEZE_NO_ACK";
    case(FunctionCode::FREEZE_CLEAR):
      return "FREEZE_CLEAR";
    case(FunctionCode::FREEZE_CLEAR_NO_ACK):
      return "FREEZE_CLEAR_NO_ACK";
    case(FunctionCode::FREEZE_AT_TIME):
      return "FREEZE_AT_TIME";
    case(FunctionCode::FREEZE_AT_TIME_NO_ACK):
      return "FREEZE_AT_TIME_NO_ACK";
    case(FunctionCode::COLD_RESTART):
      return "COLD_RESTART";
    case(FunctionCode::WARM_RESTART):
      return "WARM_RESTART";
    case(FunctionCode::INITIALIZE_DATA):
      return "INITIALIZE_DATA";
    case(FunctionCode::INITIALIZE_APPLICATION):
      return "INITIALIZE_APPLICATION";
    case(FunctionCode::START_APPLICATION):
      return "START_APPLICATION";
    case(FunctionCode::STOP_APPLICATION):
      return "STOP_APPLICATION";
    case(FunctionCode::SAVE_CONFIGURATION):
      return "SAVE_CONFIGURATION";
    case(FunctionCode::ENABLE_UNSOLICITED):
      return "ENABLE_UNSOLICITED";
    case(FunctionCode::DISABLE_UNSOLICITED):
      return "DISABLE_UNSOLICITED";
    case(FunctionCode::ASSIGN_CLASS):
      return "ASSIGN_CLASS";
    case(FunctionCode::DELAY_MEASURE):
      return "DELAY_MEASURE";
    case(FunctionCode::RECORD_TIME):
      return "RECORD_TIME";
    case(FunctionCode::FILE_OPEN):
      return "FILE_OPEN";
    case(FunctionCode::FILE_CLOSE):
      return "FILE_CLOSE";
    case(FunctionCode::FILE_DELETE):
      return "FILE_DELETE";
    case(FunctionCode::FILE_INFO):
      return "FILE_INFO";
    case(FunctionCode::FILE_AUTHENTICATE):
      return "FILE_AUTHENTICATE";
    case(FunctionCode::FILE_ABORT):
      return "FILE_ABORT";
    case(FunctionCode::RESPONSE):
      return "RESPONSE";
    case(FunctionCode::UNSOLICITED_RESPONSE):
      return "UNSOLICITED_RESPONSE";
    case(FunctionCode::UNKNOWN):
      return "UNKNOWN";
  }
  return "UNKNOWN";
}

}
