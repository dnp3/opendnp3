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

#ifndef OPENDNP3_GROUP21_H
#define OPENDNP3_GROUP21_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group21Var0
{
  static GroupVariationID ID() { return GroupVariationID(21,0); }
};

struct Group21Var1
{
  static GroupVariationID ID() { return GroupVariationID(21,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group21Var1&);
  static bool Write(const Group21Var1&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group21Var2
{
  static GroupVariationID ID() { return GroupVariationID(21,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group21Var2&);
  static bool Write(const Group21Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group21Var5
{
  static GroupVariationID ID() { return GroupVariationID(21,5); }
  static uint32_t Size() { return 11; }
  static bool Read(openpal::ReadBufferView&, Group21Var5&);
  static bool Write(const Group21Var5&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  DNPTime time;
};

struct Group21Var6
{
  static GroupVariationID ID() { return GroupVariationID(21,6); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group21Var6&);
  static bool Write(const Group21Var6&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  DNPTime time;
};

struct Group21Var9
{
  static GroupVariationID ID() { return GroupVariationID(21,9); }
  static uint32_t Size() { return 4; }
  static bool Read(openpal::ReadBufferView&, Group21Var9&);
  static bool Write(const Group21Var9&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint32_t value;
};

struct Group21Var10
{
  static GroupVariationID ID() { return GroupVariationID(21,10); }
  static uint32_t Size() { return 2; }
  static bool Read(openpal::ReadBufferView&, Group21Var10&);
  static bool Write(const Group21Var10&, openpal::WriteBufferView&);

  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef FrozenCounter Target;
  static bool ReadTarget(openpal::ReadBufferView&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint16_t value;
};


}

#endif
