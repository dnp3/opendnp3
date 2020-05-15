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

#ifndef OPENDNP3_GROUP2_H
#define OPENDNP3_GROUP2_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Binary Input Event - Any Variation
struct Group2Var0
{
  static GroupVariationID ID() { return GroupVariationID(2,0); }
};

// Binary Input Event - Without Time
struct Group2Var1
{
  static GroupVariationID ID() { return GroupVariationID(2,1); }

  Group2Var1();

  static size_t Size() { return 1; }
  static bool Read(ser4cpp::rseq_t&, Group2Var1&);
  static bool Write(const Group2Var1&, ser4cpp::wseq_t&);

  uint8_t flags;

  typedef Binary Target;
  typedef BinarySpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Binary&);
  static bool WriteTarget(const Binary&, ser4cpp::wseq_t&);
  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Binary Input Event - With Absolute Time
struct Group2Var2
{
  static GroupVariationID ID() { return GroupVariationID(2,2); }

  Group2Var2();

  static size_t Size() { return 7; }
  static bool Read(ser4cpp::rseq_t&, Group2Var2&);
  static bool Write(const Group2Var2&, ser4cpp::wseq_t&);

  uint8_t flags;
  DNPTime time;

  typedef Binary Target;
  typedef BinarySpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Binary&);
  static bool WriteTarget(const Binary&, ser4cpp::wseq_t&);
  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Binary Input Event - With Relative Time
struct Group2Var3
{
  static GroupVariationID ID() { return GroupVariationID(2,3); }

  Group2Var3();

  static size_t Size() { return 3; }
  static bool Read(ser4cpp::rseq_t&, Group2Var3&);
  static bool Write(const Group2Var3&, ser4cpp::wseq_t&);

  uint8_t flags;
  uint16_t time;

  typedef Binary Target;
  typedef BinarySpec Spec;
  static bool ReadTarget(ser4cpp::rseq_t&, Binary&);
  static bool WriteTarget(const Binary&, ser4cpp::wseq_t&);
  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
