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

#ifndef OPENDNP3_GROUP13_H
#define OPENDNP3_GROUP13_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/DNP3Serializer.h"
#include "opendnp3/app/BinaryCommandEvent.h"

namespace opendnp3 {

// Binary Command Event - Without Time
struct Group13Var1
{
  static GroupVariationID ID() { return GroupVariationID(13,1); }

  Group13Var1();

  static size_t Size() { return 1; }
  static bool Read(ser4cpp::rseq_t&, Group13Var1&);
  static bool Write(const Group13Var1&, ser4cpp::wseq_t&);

  uint8_t flags;

  typedef BinaryCommandEvent Target;
  static bool ReadTarget(ser4cpp::rseq_t&, BinaryCommandEvent&);
  static bool WriteTarget(const BinaryCommandEvent&, ser4cpp::wseq_t&);
  static DNP3Serializer<BinaryCommandEvent> Inst() { return DNP3Serializer<BinaryCommandEvent>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

// Binary Command Event - With Time
struct Group13Var2
{
  static GroupVariationID ID() { return GroupVariationID(13,2); }

  Group13Var2();

  static size_t Size() { return 7; }
  static bool Read(ser4cpp::rseq_t&, Group13Var2&);
  static bool Write(const Group13Var2&, ser4cpp::wseq_t&);

  uint8_t flags;
  DNPTime time;

  typedef BinaryCommandEvent Target;
  static bool ReadTarget(ser4cpp::rseq_t&, BinaryCommandEvent&);
  static bool WriteTarget(const BinaryCommandEvent&, ser4cpp::wseq_t&);
  static DNP3Serializer<BinaryCommandEvent> Inst() { return DNP3Serializer<BinaryCommandEvent>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
