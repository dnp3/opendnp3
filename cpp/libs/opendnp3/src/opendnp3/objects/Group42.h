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

#ifndef __OPENDNP3_GENERATED_GROUP42_H_
#define __OPENDNP3_GENERATED_GROUP42_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group42Var1
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 5;
  static Group42Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var1&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group42Var1Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var1::ID, Group42Var1::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var2
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 3;
  static Group42Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var2&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group42Var2Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var2::ID, Group42Var2::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var3
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 11;
  static Group42Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var3&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  uint64_t time;
};

struct Group42Var3Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var3::ID, Group42Var3::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var4
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 9;
  static Group42Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var4&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  uint64_t time;
};

struct Group42Var4Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var4::ID, Group42Var4::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var5
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 5;
  static Group42Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var5&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group42Var5Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var5::ID, Group42Var5::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var6
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 9;
  static Group42Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var6&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group42Var6Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var6::ID, Group42Var6::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var7
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 11;
  static Group42Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var7&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  uint64_t time;
};

struct Group42Var7Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var7::ID, Group42Var7::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group42Var8
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 15;
  static Group42Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group42Var8&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  uint64_t time;
};

struct Group42Var8Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group42Var8::ID, Group42Var8::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};


}

#endif
