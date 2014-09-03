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

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group23Var1
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 5;
  static Group23Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static FrozenCounter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const FrozenCounter&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group23Var2
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 3;
  static Group23Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static FrozenCounter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const FrozenCounter&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group23Var5
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 11;
  static Group23Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var5&, openpal::WriteBuffer&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static FrozenCounter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const FrozenCounter&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group23Var6
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 9;
  static Group23Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group23Var6&, openpal::WriteBuffer&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static FrozenCounter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const FrozenCounter&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};


}

#endif
