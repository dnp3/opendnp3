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

struct Group11Var1
{
  static const size_t SIZE = 1;
  static Group11Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var1&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group11Var2
{
  static const size_t SIZE = 7;
  static Group11Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time48;
};


}
