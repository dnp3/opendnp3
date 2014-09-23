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

#ifndef OPENDNP3_GROUP32_H
#define OPENDNP3_GROUP32_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group32Var1
{
  static GroupVariationID ID() { return GroupVariationID(32,1); }
  static uint32_t Size() { return 5; }
  static Group32Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group32Var2
{
  static GroupVariationID ID() { return GroupVariationID(32,2); }
  static uint32_t Size() { return 3; }
  static Group32Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group32Var3
{
  static GroupVariationID ID() { return GroupVariationID(32,3); }
  static uint32_t Size() { return 11; }
  static Group32Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var3&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  uint64_t time;
};

struct Group32Var4
{
  static GroupVariationID ID() { return GroupVariationID(32,4); }
  static uint32_t Size() { return 9; }
  static Group32Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var4&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  uint64_t time;
};

struct Group32Var5
{
  static GroupVariationID ID() { return GroupVariationID(32,5); }
  static uint32_t Size() { return 5; }
  static Group32Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var5&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group32Var6
{
  static GroupVariationID ID() { return GroupVariationID(32,6); }
  static uint32_t Size() { return 9; }
  static Group32Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var6&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group32Var7
{
  static GroupVariationID ID() { return GroupVariationID(32,7); }
  static uint32_t Size() { return 11; }
  static Group32Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var7&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  uint64_t time;
};

struct Group32Var8
{
  static GroupVariationID ID() { return GroupVariationID(32,8); }
  static uint32_t Size() { return 15; }
  static Group32Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var8&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  uint64_t time;
};


}

#endif
