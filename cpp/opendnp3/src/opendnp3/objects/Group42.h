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

#include <openpal/BufferWrapper.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/IDNP3Serializer.h"
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

struct Group42Var1Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var1::ID; }

  uint32_t Size() const { return Group42Var1::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var1Serializer mInstance;
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

struct Group42Var2Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var2::ID; }

  uint32_t Size() const { return Group42Var2::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var2Serializer mInstance;
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

struct Group42Var3Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var3::ID; }

  uint32_t Size() const { return Group42Var3::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var3Serializer mInstance;
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

struct Group42Var4Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var4::ID; }

  uint32_t Size() const { return Group42Var4::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var4Serializer mInstance;
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

struct Group42Var5Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var5::ID; }

  uint32_t Size() const { return Group42Var5::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var5Serializer mInstance;
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

struct Group42Var6Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var6::ID; }

  uint32_t Size() const { return Group42Var6::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var6Serializer mInstance;
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

struct Group42Var7Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var7::ID; }

  uint32_t Size() const { return Group42Var7::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var7Serializer mInstance;
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

struct Group42Var8Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group42Var8::ID; }

  uint32_t Size() const { return Group42Var8::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group42Var8Serializer mInstance;
};


}

#endif
