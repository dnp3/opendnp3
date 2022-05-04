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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#ifndef OPENDNP3_GROUP22_H
#define OPENDNP3_GROUP22_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Counter Event - Any Variation
struct Group22Var0
{
  static GroupVariationID ID() { return GroupVariationID(22,0); }
};

// Counter Event - 32-bit With Flag
struct Group22Var1
{
  static GroupVariationID ID() { return GroupVariationID(22,1); }

  Group22Var1();

  static size_t Size() { return 5; }
  static bool Read(ser4cpp::rseq_t&, Group22Var1&);
  static bool Write(const Group22Var1&, ser4cpp::wseq_t&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;

  typedef Counter Target;
  typedef CounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Counter&);
  static bool WriteTarget(const Counter&, ser4cpp::wseq_t&);
  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Counter Event - 16-bit With Flag
struct Group22Var2
{
  static GroupVariationID ID() { return GroupVariationID(22,2); }

  Group22Var2();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group22Var2&);
  static bool Write(const Group22Var2&, ser4cpp::wseq_t&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;

  typedef Counter Target;
  typedef CounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Counter&);
  static bool WriteTarget(const Counter&, ser4cpp::wseq_t&);
  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Counter Event - 32-bit With Flag and Time
struct Group22Var5
{
  static GroupVariationID ID() { return GroupVariationID(22,5); }

  Group22Var5();

  static size_t Size() { return 11; }
  static bool Read(ser4cpp::rseq_t&, Group22Var5&);
  static bool Write(const Group22Var5&, ser4cpp::wseq_t&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  DNPTime time;

  typedef Counter Target;
  typedef CounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Counter&);
  static bool WriteTarget(const Counter&, ser4cpp::wseq_t&);
  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Counter Event - 16-bit With Flag and Time
struct Group22Var6
{
  static GroupVariationID ID() { return GroupVariationID(22,6); }

  Group22Var6();

  static size_t Size() { return 9; }
  static bool Read(ser4cpp::rseq_t&, Group22Var6&);
  static bool Write(const Group22Var6&, ser4cpp::wseq_t&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  DNPTime time;

  typedef Counter Target;
  typedef CounterSpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Counter&);
  static bool WriteTarget(const Counter&, ser4cpp::wseq_t&);
  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
