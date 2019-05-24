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

#ifndef OPENDNP3_GROUP121_H
#define OPENDNP3_GROUP121_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>
#include "opendnp3/app/DNPTime.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"

namespace opendnp3 {

// Security statistic - Any Variation
struct Group121Var0
{
  static GroupVariationID ID() { return GroupVariationID(121,0); }
};

// Security statistic - 32-bit With Flag
struct Group121Var1
{
  static GroupVariationID ID() { return GroupVariationID(121,1); }

  Group121Var1();

  static uint32_t Size() { return 7; }
  static bool Read(openpal::RSlice&, Group121Var1&);
  static bool Write(const Group121Var1&, openpal::WSlice&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint16_t assocId;
  uint32_t value;

  typedef SecurityStat Target;
  typedef SecurityStatSpec Spec;
  static bool ReadTarget(openpal::RSlice&, SecurityStat&);
  static bool WriteTarget(const SecurityStat&, openpal::WSlice&);
  static DNP3Serializer<SecurityStat> Inst() { return DNP3Serializer<SecurityStat>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
