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

#ifndef __OPENDNP3_GENERATED_QUALIFIERCODE_H_
#define __OPENDNP3_GENERATED_QUALIFIERCODE_H_

#include <cstdint>

namespace opendnp3 {

enum class QualifierCode : uint8_t
{
  1B_START_STOP = 0x0,
  2B_START_STOP = 0x1,
  4B_START_STOP = 0x2,
  ALL_OBJ = 0x6,
  1B_CNT = 0x7,
  2B_CNT = 0x8,
  4B_CNT = 0x9,
  1B_CNT_1B_INDEX = 0x17,
  2B_CNT_2B_INDEX = 0x28,
  4B_CNT_4B_INDEX = 0x39,
  UNDEFINED = 0xFF
};

}

#endif
