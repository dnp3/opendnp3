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
// Copyright 2013-2020 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3_GROUP23_H
#define OPENDNP3_GROUP23_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "app/MeasurementTypeSpecs.h"

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

  Group23Var1();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group23Var1&);
  static bool Write(const Group23Var1&, ser4cpp::wseq_t&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;

  typedef FrozenCounter Target;
  typedef FrozenCounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, ser4cpp::wseq_t&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 16-bit With Flag
struct Group23Var2
{
  static GroupVariationID ID() { return GroupVariationID(23,2); }

  Group23Var2();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group23Var2&);
  static bool Write(const Group23Var2&, ser4cpp::wseq_t&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;

  typedef FrozenCounter Target;
  typedef FrozenCounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, ser4cpp::wseq_t&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 32-bit With Flag and Time
struct Group23Var5
{
  static GroupVariationID ID() { return GroupVariationID(23,5); }

  Group23Var5();

  static size_t Size() { return 11; }
  static bool Read(ser4cpp::rseq_t&, Group23Var5&);
  static bool Write(const Group23Var5&, ser4cpp::wseq_t&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  DNPTime time;

  typedef FrozenCounter Target;
  typedef FrozenCounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, ser4cpp::wseq_t&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Frozen Counter Event - 16-bit With Flag and Time
struct Group23Var6
{
  static GroupVariationID ID() { return GroupVariationID(23,6); }

  Group23Var6();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group23Var6&);
  static bool Write(const Group23Var6&, ser4cpp::wseq_t&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  DNPTime time;

  typedef FrozenCounter Target;
  typedef FrozenCounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, FrozenCounter&);
  static bool WriteTarget(const FrozenCounter&, ser4cpp::wseq_t&);
  static DNP3Serializer<FrozenCounter> Inst() { return DNP3Serializer<FrozenCounter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
