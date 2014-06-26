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

#ifndef __OPENDNP3_GENERATED_GROUP20_H_
#define __OPENDNP3_GENERATED_GROUP20_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/IDNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group20Var1
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 5;
  static Group20Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var1&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group20Var1Serializer : public IDNP3Serializer<Counter>
{

  static IDNP3Serializer<Counter>& Inst() { return instance; }

  GroupVariationID ID() const { return Group20Var1::ID; }

  uint32_t Size() const { return Group20Var1::SIZE; }

  typedef Counter Target;
  Counter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const Counter&, openpal::WriteBuffer&) const;

  private:

  static Group20Var1Serializer instance;
};

struct Group20Var2
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 3;
  static Group20Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var2&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group20Var2Serializer : public IDNP3Serializer<Counter>
{

  static IDNP3Serializer<Counter>& Inst() { return instance; }

  GroupVariationID ID() const { return Group20Var2::ID; }

  uint32_t Size() const { return Group20Var2::SIZE; }

  typedef Counter Target;
  Counter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const Counter&, openpal::WriteBuffer&) const;

  private:

  static Group20Var2Serializer instance;
};

struct Group20Var5
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 4;
  static Group20Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var5&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint32_t value;
};

struct Group20Var5Serializer : public IDNP3Serializer<Counter>
{

  static IDNP3Serializer<Counter>& Inst() { return instance; }

  GroupVariationID ID() const { return Group20Var5::ID; }

  uint32_t Size() const { return Group20Var5::SIZE; }

  typedef Counter Target;
  Counter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const Counter&, openpal::WriteBuffer&) const;

  private:

  static Group20Var5Serializer instance;
};

struct Group20Var6
{
  static const GroupVariationID ID;
  typedef Counter Target;
  static const uint32_t SIZE = 2;
  static Group20Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var6&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint16_t value;
};

struct Group20Var6Serializer : public IDNP3Serializer<Counter>
{

  static IDNP3Serializer<Counter>& Inst() { return instance; }

  GroupVariationID ID() const { return Group20Var6::ID; }

  uint32_t Size() const { return Group20Var6::SIZE; }

  typedef Counter Target;
  Counter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const Counter&, openpal::WriteBuffer&) const;

  private:

  static Group20Var6Serializer instance;
};


}

#endif
