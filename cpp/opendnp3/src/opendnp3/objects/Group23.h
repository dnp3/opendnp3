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

#ifndef __OPENDNP3_GENERATED_GROUP23_H_
#define __OPENDNP3_GENERATED_GROUP23_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"

namespace opendnp3 {

struct Group23Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;
  static Group23Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var1&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group23Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;
  static Group23Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var2&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group23Var3
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;
  static Group23Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var3&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group23Var4
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;
  static Group23Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var4&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group23Var5
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;
  static Group23Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var5&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group23Var6
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;
  static Group23Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var6&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};

struct Group23Var7
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;
  static Group23Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var7&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group23Var8
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;
  static Group23Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var8&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};


}

#endif
