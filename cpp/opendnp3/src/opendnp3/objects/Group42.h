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

struct Group42Var1
{
  static const size_t SIZE = 5;
  static Group42Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
};

struct Group42Var2
{
  static const size_t SIZE = 3;
  static Group42Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
};

struct Group42Var3
{
  static const size_t SIZE = 11;
  static Group42Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
  uint64_t time48;
};

struct Group42Var4
{
  static const size_t SIZE = 9;
  static Group42Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
  uint64_t time48;
};

struct Group42Var5
{
  static const size_t SIZE = 5;
  static Group42Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var5&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
};

struct Group42Var6
{
  static const size_t SIZE = 9;
  static Group42Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var6&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
};

struct Group42Var7
{
  static const size_t SIZE = 11;
  static Group42Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var7&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
  uint64_t time48;
};

struct Group42Var8
{
  static const size_t SIZE = 15;
  static Group42Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var8&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
  uint64_t time48;
};


}
