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

#ifndef __OPENDNP3_GENERATED_GROUP22_H_
#define __OPENDNP3_GENERATED_GROUP22_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group22Var1
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 5;
  static Group22Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var1&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group22Var1Serializer
{

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(Group22Var1::ID, Group22Var1::SIZE, &Read, &Write); }

  typedef Counter Target;
  static Counter Read(openpal::ReadOnlyBuffer&);
  static void Write(const Counter&, openpal::WriteBuffer&);

};

struct Group22Var2
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 3;
  static Group22Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var2&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group22Var2Serializer
{

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(Group22Var2::ID, Group22Var2::SIZE, &Read, &Write); }

  typedef Counter Target;
  static Counter Read(openpal::ReadOnlyBuffer&);
  static void Write(const Counter&, openpal::WriteBuffer&);

};

struct Group22Var5
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 11;
  static Group22Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var5&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group22Var5Serializer
{

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(Group22Var5::ID, Group22Var5::SIZE, &Read, &Write); }

  typedef Counter Target;
  static Counter Read(openpal::ReadOnlyBuffer&);
  static void Write(const Counter&, openpal::WriteBuffer&);

};

struct Group22Var6
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 9;
  static Group22Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group22Var6&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};

struct Group22Var6Serializer
{

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(Group22Var6::ID, Group22Var6::SIZE, &Read, &Write); }

  typedef Counter Target;
  static Counter Read(openpal::ReadOnlyBuffer&);
  static void Write(const Counter&, openpal::WriteBuffer&);

};


}

#endif
