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

#ifndef OPENDNP3_GROUP42_H
#define OPENDNP3_GROUP42_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group42Var1
{
  static GroupVariationID ID() { return GroupVariationID(42,1); }
  static uint32_t Size() { return 5; }
  static Group42Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group42Var2
{
  static GroupVariationID ID() { return GroupVariationID(42,2); }
  static uint32_t Size() { return 3; }
  static Group42Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group42Var3
{
  static GroupVariationID ID() { return GroupVariationID(42,3); }
  static uint32_t Size() { return 11; }
  static Group42Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var3&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  uint64_t time;
};

struct Group42Var4
{
  static GroupVariationID ID() { return GroupVariationID(42,4); }
  static uint32_t Size() { return 9; }
  static Group42Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var4&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  uint64_t time;
};

struct Group42Var5
{
  static GroupVariationID ID() { return GroupVariationID(42,5); }
  static uint32_t Size() { return 5; }
  static Group42Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var5&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group42Var6
{
  static GroupVariationID ID() { return GroupVariationID(42,6); }
  static uint32_t Size() { return 9; }
  static Group42Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var6&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group42Var7
{
  static GroupVariationID ID() { return GroupVariationID(42,7); }
  static uint32_t Size() { return 11; }
  static Group42Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var7&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  uint64_t time;
};

struct Group42Var8
{
  static GroupVariationID ID() { return GroupVariationID(42,8); }
  static uint32_t Size() { return 15; }
  static Group42Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var8&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputStatus&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  uint64_t time;
};


}

#endif
