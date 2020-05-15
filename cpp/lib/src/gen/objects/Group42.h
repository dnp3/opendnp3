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

#ifndef OPENDNP3_GROUP42_H
#define OPENDNP3_GROUP42_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Analog Output Event - Any Variation
struct Group42Var0
{
  static GroupVariationID ID() { return GroupVariationID(42,0); }
};

// Analog Output Event - 32-bit With Flag
struct Group42Var1
{
  static GroupVariationID ID() { return GroupVariationID(42,1); }

  Group42Var1();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group42Var1&);
  static bool Write(const Group42Var1&, ser4cpp::wseq_t&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - 16-bit With Flag
struct Group42Var2
{
  static GroupVariationID ID() { return GroupVariationID(42,2); }

  Group42Var2();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group42Var2&);
  static bool Write(const Group42Var2&, ser4cpp::wseq_t&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - 32-bit With Flag and Time
struct Group42Var3
{
  static GroupVariationID ID() { return GroupVariationID(42,3); }

  Group42Var3();

  static size_t Size() { return 11; }
  static bool Read(ser4cpp::rseq_t&, Group42Var3&);
  static bool Write(const Group42Var3&, ser4cpp::wseq_t&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  DNPTime time;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - 16-bit With Flag and Time
struct Group42Var4
{
  static GroupVariationID ID() { return GroupVariationID(42,4); }

  Group42Var4();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group42Var4&);
  static bool Write(const Group42Var4&, ser4cpp::wseq_t&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  DNPTime time;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - Single-precision With Flag
struct Group42Var5
{
  static GroupVariationID ID() { return GroupVariationID(42,5); }

  Group42Var5();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group42Var5&);
  static bool Write(const Group42Var5&, ser4cpp::wseq_t&);

  typedef float ValueType;
  uint8_t flags;
  float value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - Double-precision With Flag
struct Group42Var6
{
  static GroupVariationID ID() { return GroupVariationID(42,6); }

  Group42Var6();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group42Var6&);
  static bool Write(const Group42Var6&, ser4cpp::wseq_t&);

  typedef double ValueType;
  uint8_t flags;
  double value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - Single-precision With Flag and Time
struct Group42Var7
{
  static GroupVariationID ID() { return GroupVariationID(42,7); }

  Group42Var7();

  static size_t Size() { return 11; }
  static bool Read(ser4cpp::rseq_t&, Group42Var7&);
  static bool Write(const Group42Var7&, ser4cpp::wseq_t&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  DNPTime time;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Event - Double-precision With Flag and Time
struct Group42Var8
{
  static GroupVariationID ID() { return GroupVariationID(42,8); }

  Group42Var8();

  static size_t Size() { return 15; }
  static bool Read(ser4cpp::rseq_t&, Group42Var8&);
  static bool Write(const Group42Var8&, ser4cpp::wseq_t&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  DNPTime time;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
