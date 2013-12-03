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

#include <openpal/BufferWrapper.h>

namespace opendnp3 {

struct Group2Var1
{
  static Group2Var1 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
};

struct Group2Var2
{
  static Group2Var2 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint64_t time48;
};

struct Group2Var3
{
  static Group2Var3 Read(openpal::ReadOnlyBuffer&);

  uint8_t flags;
  uint16_t time16;
};


}
