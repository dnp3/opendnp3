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

#ifndef OPENDNP3_GROUP41_H
#define OPENDNP3_GROUP41_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/gen/CommandStatus.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3 {

struct Group41Var1
{
  static GroupVariationID ID() { return GroupVariationID(41,1); }
  static uint32_t Size() { return 5; }
  static Group41Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputInt32> Inst() { return DNP3Serializer<AnalogOutputInt32>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputInt32 Target;
  static AnalogOutputInt32 ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputInt32&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  int32_t value;
  CommandStatus status;
};

struct Group41Var2
{
  static GroupVariationID ID() { return GroupVariationID(41,2); }
  static uint32_t Size() { return 3; }
  static Group41Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputInt16> Inst() { return DNP3Serializer<AnalogOutputInt16>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputInt16 Target;
  static AnalogOutputInt16 ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputInt16&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  int16_t value;
  CommandStatus status;
};

struct Group41Var3
{
  static GroupVariationID ID() { return GroupVariationID(41,3); }
  static uint32_t Size() { return 5; }
  static Group41Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var3&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputFloat32> Inst() { return DNP3Serializer<AnalogOutputFloat32>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputFloat32 Target;
  static AnalogOutputFloat32 ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputFloat32&, openpal::WriteBuffer&);

  typedef float ValueType;
  float value;
  CommandStatus status;
};

struct Group41Var4
{
  static GroupVariationID ID() { return GroupVariationID(41,4); }
  static uint32_t Size() { return 9; }
  static Group41Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var4&, openpal::WriteBuffer&);

  static DNP3Serializer<AnalogOutputDouble64> Inst() { return DNP3Serializer<AnalogOutputDouble64>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef AnalogOutputDouble64 Target;
  static AnalogOutputDouble64 ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const AnalogOutputDouble64&, openpal::WriteBuffer&);

  typedef double ValueType;
  double value;
  CommandStatus status;
};


}

#endif
