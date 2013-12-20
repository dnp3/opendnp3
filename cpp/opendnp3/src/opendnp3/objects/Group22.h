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

#ifndef __OPENDNP3_GENERATED_GROUP22_H_
#define __OPENDNP3_GENERATED_GROUP22_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group22Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group22Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group22Var3
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group22Var4
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group22Var5
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var5&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group22Var6
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var6&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};

struct Group22Var7
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var7&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
  uint64_t time48;
};

struct Group22Var8
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group22Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var8&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
  uint64_t time48;
};


}

#endif
