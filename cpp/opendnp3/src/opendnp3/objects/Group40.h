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

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include "IDNP3Serializer.h"
#include "../DataTypes.h"

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

struct Group40Var1Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group40Var1::ID; }

  uint32_t Size() const { return Group40Var1::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group40Var1Serializer mInstance;
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

struct Group40Var2Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group40Var2::ID; }

  uint32_t Size() const { return Group40Var2::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group40Var2Serializer mInstance;
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

struct Group40Var3Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group40Var3::ID; }

  uint32_t Size() const { return Group40Var3::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group40Var3Serializer mInstance;
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

struct Group40Var4Serializer : public IDNP3Serializer<AnalogOutputStatus>
{

  static IDNP3Serializer<AnalogOutputStatus>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group40Var4::ID; }

  uint32_t Size() const { return Group40Var4::SIZE; }

  typedef AnalogOutputStatus Target;
  AnalogOutputStatus Read(openpal::ReadOnlyBuffer&) const;
  void Write(const AnalogOutputStatus&, openpal::WriteBuffer&) const;

  private:

  static Group40Var4Serializer mInstance;
};


}

#endif
