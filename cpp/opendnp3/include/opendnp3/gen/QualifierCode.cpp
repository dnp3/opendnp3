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

#include "QualifierCode.h"

namespace opendnp3 {

std::string QualifierCodeToString(QualifierCode arg)
{
  switch(arg)
  {
    case(QualifierCode::UINT8_START_STOP):
      return "UINT8_START_STOP";
    case(QualifierCode::UINT16_START_STOP):
      return "UINT16_START_STOP";
    case(QualifierCode::UINT32_START_STOP):
      return "UINT32_START_STOP";
    case(QualifierCode::ALL_OBJECTS):
      return "ALL_OBJECTS";
    case(QualifierCode::UINT8_CNT):
      return "UINT8_CNT";
    case(QualifierCode::UINT16_CNT):
      return "UINT16_CNT";
    case(QualifierCode::UINT32_CNT):
      return "UINT32_CNT";
    case(QualifierCode::UINT8_CNT_UINT8_INDEX):
      return "UINT8_CNT_UINT8_INDEX";
    case(QualifierCode::UINT16_CNT_UINT16_INDEX):
      return "UINT16_CNT_UINT16_INDEX";
    case(QualifierCode::UINT32_CNT_UINT32_INDEX):
      return "UINT32_CNT_UINT32_INDEX";
    case(QualifierCode::UNDEFINED):
      return "UNDEFINED";
  }
  return "UNDEFINED";
}

}
