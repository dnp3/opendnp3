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

#ifndef __OPENDNP3_GENERATED_GROUP32_H_
#define __OPENDNP3_GENERATED_GROUP32_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group32Var1
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 5;
  static Group32Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var1&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group32Var1Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var1::ID, Group32Var1::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var2
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 3;
  static Group32Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var2&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group32Var2Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var2::ID, Group32Var2::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var3
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 11;
  static Group32Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var3&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  uint64_t time;
};

struct Group32Var3Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var3::ID, Group32Var3::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var4
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 9;
  static Group32Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var4&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  uint64_t time;
};

struct Group32Var4Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var4::ID, Group32Var4::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var5
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 5;
  static Group32Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var5&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group32Var5Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var5::ID, Group32Var5::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var6
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 9;
  static Group32Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var6&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group32Var6Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var6::ID, Group32Var6::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var7
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 11;
  static Group32Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var7&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  uint64_t time;
};

struct Group32Var7Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var7::ID, Group32Var7::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};

struct Group32Var8
{
  static const GroupVariationID ID;
  typedef Analog Target;
  static const uint32_t SIZE = 15;
  static Group32Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var8&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  uint64_t time;
};

struct Group32Var8Serializer
{

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(Group32Var8::ID, Group32Var8::SIZE, &Read, &Write); }

  typedef Analog Target;
  static Analog Read(openpal::ReadOnlyBuffer&);
  static void Write(const Analog&, openpal::WriteBuffer&);

};


}

#endif
