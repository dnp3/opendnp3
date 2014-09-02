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

#ifndef __OPENDNP3_GENERATED_GROUP40_H_
#define __OPENDNP3_GENERATED_GROUP40_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group40Var1
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 5;
  static Group40Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var1&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group40Var1Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group40Var1::ID, Group40Var1::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group40Var2
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 3;
  static Group40Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var2&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group40Var2Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group40Var2::ID, Group40Var2::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group40Var3
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 5;
  static Group40Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var3&, openpal::WriteBuffer&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group40Var3Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group40Var3::ID, Group40Var3::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};

struct Group40Var4
{
  static const GroupVariationID ID;
  typedef AnalogOutputStatus Target;
  static const uint32_t SIZE = 9;
  static Group40Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var4&, openpal::WriteBuffer&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group40Var4Serializer
{

  static DNP3Serializer<AnalogOutputStatus> Inst() { return DNP3Serializer<AnalogOutputStatus>(Group40Var4::ID, Group40Var4::SIZE, &Read, &Write); }

  typedef AnalogOutputStatus Target;
  static AnalogOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputStatus&, openpal::WriteBuffer&);

};


}

#endif
