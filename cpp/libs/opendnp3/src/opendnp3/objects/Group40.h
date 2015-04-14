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

#ifndef OPENDNP3_GROUP40_H
#define OPENDNP3_GROUP40_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group40Var0
{
  static GroupVariationID ID() { return GroupVariationID(40,0); }
};

struct Group40Var1
{
  static GroupVariationID ID() { return GroupVariationID(40,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group40Var1&);
  static bool Write(const Group40Var1&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;

  typedef AnalogOutputStatus Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

struct Group40Var2
{
  static GroupVariationID ID() { return GroupVariationID(40,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group40Var2&);
  static bool Write(const Group40Var2&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;

  typedef AnalogOutputStatus Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

struct Group40Var3
{
  static GroupVariationID ID() { return GroupVariationID(40,3); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group40Var3&);
  static bool Write(const Group40Var3&, openpal::WriteBufferView&);

  typedef float ValueType;
  uint8_t flags;
  float value;

  typedef AnalogOutputStatus Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};

struct Group40Var4
{
  static GroupVariationID ID() { return GroupVariationID(40,4); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group40Var4&);
  static bool Write(const Group40Var4&, openpal::WriteBufferView&);

  typedef double ValueType;
  uint8_t flags;
  double value;

  typedef AnalogOutputStatus Target;
  static bool ReadTarget(openpal::ReadBufferView&, AnalogOutputStatus&);
  static bool WriteTarget(const AnalogOutputStatus&, openpal::WriteBufferView&);
  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }
};


}

#endif
