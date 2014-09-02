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

#ifndef __OPENDNP3_GENERATED_GROUP2_H_
#define __OPENDNP3_GENERATED_GROUP2_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group2Var0
{
  static const GroupVariationID ID;
};

struct Group2Var1
{
  static const GroupVariationID ID;
  typedef Binary Target;
  static const uint32_t SIZE = 1;
  static Group2Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var1&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group2Var1Serializer
{

  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(Group2Var1::ID, Group2Var1::SIZE, &Read, &Write); }

  typedef Binary Target;
  static Binary Read(openpal::ReadOnlyBuffer&);
  static void Write(const Binary&, openpal::WriteBuffer&);

};

struct Group2Var2
{
  static const GroupVariationID ID;
  typedef Binary Target;
  static const uint32_t SIZE = 7;
  static Group2Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time;
};

struct Group2Var2Serializer
{

  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(Group2Var2::ID, Group2Var2::SIZE, &Read, &Write); }

  typedef Binary Target;
  static Binary Read(openpal::ReadOnlyBuffer&);
  static void Write(const Binary&, openpal::WriteBuffer&);

};

struct Group2Var3
{
  static const GroupVariationID ID;
  typedef Binary Target;
  static const uint32_t SIZE = 3;
  static Group2Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t time;
};

struct Group2Var3Serializer
{

  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(Group2Var3::ID, Group2Var3::SIZE, &Read, &Write); }

  typedef Binary Target;
  static Binary Read(openpal::ReadOnlyBuffer&);
  static void Write(const Binary&, openpal::WriteBuffer&);

};


}

#endif
