//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef __OPENDNP3_GENERATED_GROUP30_H_
#define __OPENDNP3_GENERATED_GROUP30_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group30Var1
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 5;
  static Group30Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group30Var2
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 3;
  static Group30Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group30Var3
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 4;
  static Group30Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var3&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  int32_t value;
};

struct Group30Var4
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 2;
  static Group30Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var4&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  int16_t value;
};

struct Group30Var5
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 5;
  static Group30Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var5&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group30Var6
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 9;
  static Group30Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group30Var6&, openpal::WriteBuffer&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static Analog ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Analog&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};


}

#endif
