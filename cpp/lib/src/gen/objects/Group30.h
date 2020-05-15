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

#ifndef OPENDNP3_GROUP30_H
#define OPENDNP3_GROUP30_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Analog Input - Any Variation
struct Group30Var0
{
  static GroupVariationID ID() { return GroupVariationID(30,0); }
};

// Analog Input - 32-bit With Flag
struct Group30Var1
{
  static GroupVariationID ID() { return GroupVariationID(30,1); }

  Group30Var1();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group30Var1&);
  static bool Write(const Group30Var1&, ser4cpp::wseq_t&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var1;
};

// Analog Input - 16-bit With Flag
struct Group30Var2
{
  static GroupVariationID ID() { return GroupVariationID(30,2); }

  Group30Var2();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group30Var2&);
  static bool Write(const Group30Var2&, ser4cpp::wseq_t&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var2;
};

// Analog Input - 32-bit Without Flag
struct Group30Var3
{
  static GroupVariationID ID() { return GroupVariationID(30,3); }

  Group30Var3();

  static size_t Size() { return 4; }
  static bool Read(ser4cpp::rseq_t&, Group30Var3&);
  static bool Write(const Group30Var3&, ser4cpp::wseq_t&);

  typedef int32_t ValueType;
  int32_t value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var3;
};

// Analog Input - 16-bit Without Flag
struct Group30Var4
{
  static GroupVariationID ID() { return GroupVariationID(30,4); }

  Group30Var4();

  static size_t Size() { return 2; }
  static bool Read(ser4cpp::rseq_t&, Group30Var4&);
  static bool Write(const Group30Var4&, ser4cpp::wseq_t&);

  typedef int16_t ValueType;
  int16_t value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var4;
};

// Analog Input - Single-precision With Flag
struct Group30Var5
{
  static GroupVariationID ID() { return GroupVariationID(30,5); }

  Group30Var5();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group30Var5&);
  static bool Write(const Group30Var5&, ser4cpp::wseq_t&);

  typedef float ValueType;
  uint8_t flags;
  float value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var5;
};

// Analog Input - Double-precision With Flag
struct Group30Var6
{
  static GroupVariationID ID() { return GroupVariationID(30,6); }

  Group30Var6();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group30Var6&);
  static bool Write(const Group30Var6&, ser4cpp::wseq_t&);

  typedef double ValueType;
  uint8_t flags;
  double value;

  typedef Analog Target;
  typedef AnalogSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Analog&);
  static bool WriteTarget(const Analog&, ser4cpp::wseq_t&);
  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }
  static const StaticAnalogVariation svariation = StaticAnalogVariation::Group30Var6;
};


}

#endif
