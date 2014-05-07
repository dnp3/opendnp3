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

#ifndef __OPENDNP3_GENERATED_GROUP21_H_
#define __OPENDNP3_GENERATED_GROUP21_H_

#include <openpal/BufferWrapper.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/IDNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group21Var1
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 5;
  static Group21Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var1&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group21Var1Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var1::ID; }

  uint32_t Size() const { return Group21Var1::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var1Serializer mInstance;
};

struct Group21Var2
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 3;
  static Group21Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var2&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group21Var2Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var2::ID; }

  uint32_t Size() const { return Group21Var2::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var2Serializer mInstance;
};

struct Group21Var5
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 11;
  static Group21Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var5&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  uint64_t time;
};

struct Group21Var5Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var5::ID; }

  uint32_t Size() const { return Group21Var5::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var5Serializer mInstance;
};

struct Group21Var6
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 9;
  static Group21Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var6&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  uint64_t time;
};

struct Group21Var6Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var6::ID; }

  uint32_t Size() const { return Group21Var6::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var6Serializer mInstance;
};

struct Group21Var9
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 4;
  static Group21Var9 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var9&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint32_t value;
};

struct Group21Var9Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var9::ID; }

  uint32_t Size() const { return Group21Var9::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var9Serializer mInstance;
};

struct Group21Var10
{
  static const GroupVariationID ID;
  typedef FrozenCounter Target;
  static const uint32_t SIZE = 2;
  static Group21Var10 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group21Var10&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint16_t value;
};

struct Group21Var10Serializer : public IDNP3Serializer<FrozenCounter>
{

  static IDNP3Serializer<FrozenCounter>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group21Var10::ID; }

  uint32_t Size() const { return Group21Var10::SIZE; }

  typedef FrozenCounter Target;
  FrozenCounter Read(openpal::ReadOnlyBuffer&) const;
  void Write(const FrozenCounter&, openpal::WriteBuffer&) const;

  private:

  static Group21Var10Serializer mInstance;
};


}

#endif
