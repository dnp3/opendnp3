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

#ifndef OPENDNP3_GROUP41_H
#define OPENDNP3_GROUP41_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3 {

// Analog Output - Any Variation
struct Group41Var0
{
  static GroupVariationID ID() { return GroupVariationID(41,0); }
};

// Analog Output - 32-bit With Flag
struct Group41Var1
{
  static GroupVariationID ID() { return GroupVariationID(41,1); }

  Group41Var1();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group41Var1&);
  static bool Write(const Group41Var1&, ser4cpp::wseq_t&);

  typedef int32_t ValueType;
  int32_t value;
  uint8_t status;

  typedef AnalogOutputInt32 Target;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputInt32&);
  static bool WriteTarget(const AnalogOutputInt32&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputInt32> Inst() { return DNP3Serializer<AnalogOutputInt32>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - 16-bit With Flag
struct Group41Var2
{
  static GroupVariationID ID() { return GroupVariationID(41,2); }

  Group41Var2();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group41Var2&);
  static bool Write(const Group41Var2&, ser4cpp::wseq_t&);

  typedef int16_t ValueType;
  int16_t value;
  uint8_t status;

  typedef AnalogOutputInt16 Target;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputInt16&);
  static bool WriteTarget(const AnalogOutputInt16&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputInt16> Inst() { return DNP3Serializer<AnalogOutputInt16>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - Single-precision
struct Group41Var3
{
  static GroupVariationID ID() { return GroupVariationID(41,3); }

  Group41Var3();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group41Var3&);
  static bool Write(const Group41Var3&, ser4cpp::wseq_t&);

  typedef float ValueType;
  float value;
  uint8_t status;

  typedef AnalogOutputFloat32 Target;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputFloat32&);
  static bool WriteTarget(const AnalogOutputFloat32&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputFloat32> Inst() { return DNP3Serializer<AnalogOutputFloat32>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output - Double-precision
struct Group41Var4
{
  static GroupVariationID ID() { return GroupVariationID(41,4); }

  Group41Var4();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group41Var4&);
  static bool Write(const Group41Var4&, ser4cpp::wseq_t&);

  typedef double ValueType;
  double value;
  uint8_t status;

  typedef AnalogOutputDouble64 Target;
  static bool ReadTarget(ser4cpp::rseq_t&, AnalogOutputDouble64&);
  static bool WriteTarget(const AnalogOutputDouble64&, ser4cpp::wseq_t&);
  static DNP3Serializer<AnalogOutputDouble64> Inst() { return DNP3Serializer<AnalogOutputDouble64>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
