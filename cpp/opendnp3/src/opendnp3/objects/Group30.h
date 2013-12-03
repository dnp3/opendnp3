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

struct Group30Var1
{
  static Group30Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
};

struct Group30Var2
{
  static Group30Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
};

struct Group30Var3
{
  static Group30Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var3&, openpal::WriteBuffer&);

  int32_t value;
};

struct Group30Var4
{
  static Group30Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var4&, openpal::WriteBuffer&);

  int16_t value;
};

struct Group30Var5
{
  static Group30Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var5&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
};

struct Group30Var6
{
  static Group30Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var6&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
};


}
