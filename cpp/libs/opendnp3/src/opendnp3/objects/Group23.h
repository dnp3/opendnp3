//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef OPENDNP3_GROUP23_H
#define OPENDNP3_GROUP23_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

// Frozen Counter Event - Any Variation
struct Group23Var0
{
  static GroupVariationID ID() { return GroupVariationID(23,0); }
};

// Frozen Counter Event - 32-bit With Flag
struct Group23Var1
{
  static GroupVariationID ID() { return GroupVariationID(23,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group23Var1&);
  static bool Write(const Group23Var1&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 16-bit With Flag
struct Group23Var2
{
  static GroupVariationID ID() { return GroupVariationID(23,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group23Var2&);
  static bool Write(const Group23Var2&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 32-bit With Flag and Time
struct Group23Var5
{
  static GroupVariationID ID() { return GroupVariationID(23,5); }
  static uint32_t Size() { return 11; }
  static bool Read(openpal::ReadBufferView&, Group23Var5&);
  static bool Write(const Group23Var5&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  DNPTime time;

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 16-bit With Flag and Time
struct Group23Var6
{
  static GroupVariationID ID() { return GroupVariationID(23,6); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group23Var6&);
  static bool Write(const Group23Var6&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  DNPTime time;

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
