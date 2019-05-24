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
// Copyright 2013-2019 Automatak, LLC
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

#ifndef OPENDNP3_GROUP40_H
#define OPENDNP3_GROUP40_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>
#include "opendnp3/app/DNPTime.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Analog Output Status - Any Variation
struct Group40Var0
{
  static GroupVariationID ID() { return GroupVariationID(40,0); }
};

// Analog Output Status - 32-bit With Flag
struct Group40Var1
{
  static GroupVariationID ID() { return GroupVariationID(40,1); }

  Group40Var1();

  static uint32_t Size() { return 5; }
  static bool Read(openpal::RSlice&, Group40Var1&);
  static bool Write(const Group40Var1&, openpal::WSlice&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(openpal::RSlice&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WSlice&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Status - 16-bit With Flag
struct Group40Var2
{
  static GroupVariationID ID() { return GroupVariationID(40,2); }

  Group40Var2();

  static uint32_t Size() { return 3; }
  static bool Read(openpal::RSlice&, Group40Var2&);
  static bool Write(const Group40Var2&, openpal::WSlice&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(openpal::RSlice&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WSlice&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Status - Single-precision With Flag
struct Group40Var3
{
  static GroupVariationID ID() { return GroupVariationID(40,3); }

  Group40Var3();

  static uint32_t Size() { return 5; }
  static bool Read(openpal::RSlice&, Group40Var3&);
  static bool Write(const Group40Var3&, openpal::WSlice&);

  typedef float ValueType;
  uint8_t flags;
  float value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(openpal::RSlice&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WSlice&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Analog Output Status - Double-precision With Flag
struct Group40Var4
{
  static GroupVariationID ID() { return GroupVariationID(40,4); }

  Group40Var4();

  static uint32_t Size() { return 9; }
  static bool Read(openpal::RSlice&, Group40Var4&);
  static bool Write(const Group40Var4&, openpal::WSlice&);

  typedef double ValueType;
  uint8_t flags;
  double value;

  typedef AnalogOutputStatus Target;
  typedef AnalogOutputStatusSpec Spec;
  static bool ReadTarget(openpal::RSlice&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WSlice&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
