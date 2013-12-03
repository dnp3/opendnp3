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

struct Group21Var1
{
  static Group21Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group21Var2
{
  static Group21Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group21Var3
{
  static Group21Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group21Var4
{
  static Group21Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group21Var5
{
  static Group21Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var5&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group21Var6
{
  static Group21Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var6&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};

struct Group21Var7
{
  static Group21Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var7&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group21Var8
{
  static Group21Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var8&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};

struct Group21Var9
{
  static Group21Var9 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var9&, openpal::WriteBuffer&);

  uint16_t count;
};

struct Group21Var10
{
  static Group21Var10 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var10&, openpal::WriteBuffer&);

  uint32_t count;
};

struct Group21Var11
{
  static Group21Var11 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var11&, openpal::WriteBuffer&);

  uint16_t count;
};

struct Group21Var12
{
  static Group21Var12 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var12&, openpal::WriteBuffer&);

  uint32_t count;
};


}
