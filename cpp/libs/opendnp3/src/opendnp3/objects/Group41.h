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

#ifndef __OPENDNP3_GENERATED_GROUP41_H_
#define __OPENDNP3_GENERATED_GROUP41_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/gen/CommandStatus.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/AnalogOutput.h"

namespace opendnp3 {

struct Group41Var1
{
  static const GroupVariationID ID;
  typedef AnalogOutputInt32 Target;
  static const uint32_t SIZE = 5;
  static Group41Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var1&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  int32_t value;
  CommandStatus status;
};

struct Group41Var1Serializer
{

  static DNP3Serializer<AnalogOutputInt32> Inst() { return DNP3Serializer<AnalogOutputInt32>(Group41Var1::ID, Group41Var1::SIZE, &Read, &Write); }

  typedef AnalogOutputInt32 Target;
  static AnalogOutputInt32 Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputInt32&, openpal::WriteBuffer&);

};

struct Group41Var2
{
  static const GroupVariationID ID;
  typedef AnalogOutputInt16 Target;
  static const uint32_t SIZE = 3;
  static Group41Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var2&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  int16_t value;
  CommandStatus status;
};

struct Group41Var2Serializer
{

  static DNP3Serializer<AnalogOutputInt16> Inst() { return DNP3Serializer<AnalogOutputInt16>(Group41Var2::ID, Group41Var2::SIZE, &Read, &Write); }

  typedef AnalogOutputInt16 Target;
  static AnalogOutputInt16 Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputInt16&, openpal::WriteBuffer&);

};

struct Group41Var3
{
  static const GroupVariationID ID;
  typedef AnalogOutputFloat32 Target;
  static const uint32_t SIZE = 5;
  static Group41Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var3&, openpal::WriteBuffer&);

  typedef float ValueType;
  float value;
  CommandStatus status;
};

struct Group41Var3Serializer
{

  static DNP3Serializer<AnalogOutputFloat32> Inst() { return DNP3Serializer<AnalogOutputFloat32>(Group41Var3::ID, Group41Var3::SIZE, &Read, &Write); }

  typedef AnalogOutputFloat32 Target;
  static AnalogOutputFloat32 Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputFloat32&, openpal::WriteBuffer&);

};

struct Group41Var4
{
  static const GroupVariationID ID;
  typedef AnalogOutputDouble64 Target;
  static const uint32_t SIZE = 9;
  static Group41Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var4&, openpal::WriteBuffer&);

  typedef double ValueType;
  double value;
  CommandStatus status;
};

struct Group41Var4Serializer
{

  static DNP3Serializer<AnalogOutputDouble64> Inst() { return DNP3Serializer<AnalogOutputDouble64>(Group41Var4::ID, Group41Var4::SIZE, &Read, &Write); }

  typedef AnalogOutputDouble64 Target;
  static AnalogOutputDouble64 Read(openpal::ReadOnlyBuffer&);
  static void Write(const AnalogOutputDouble64&, openpal::WriteBuffer&);

};


}

#endif
