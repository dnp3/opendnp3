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

#ifndef __OPENDNP3_GENERATED_GROUP21_H_
#define __OPENDNP3_GENERATED_GROUP21_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"

namespace opendnp3 {

struct Group21Var1
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 5;
  static Group21Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var1&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group21Var2
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 3;
  static Group21Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var2&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group21Var3
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 5;
  static Group21Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var3&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group21Var4
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 3;
  static Group21Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var4&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group21Var5
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 11;
  static Group21Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var5&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group21Var6
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 9;
  static Group21Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var6&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};

struct Group21Var7
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 11;
  static Group21Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var7&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group21Var8
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 9;
  static Group21Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var8&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};

struct Group21Var9
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 2;
  static Group21Var9 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var9&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint16_t value;
};

struct Group21Var10
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 4;
  static Group21Var10 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var10&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint32_t value;
};

struct Group21Var11
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 2;
  static Group21Var11 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var11&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint16_t value;
};

struct Group21Var12
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 4;
  static Group21Var12 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var12&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint32_t value;
};


}

#endif
