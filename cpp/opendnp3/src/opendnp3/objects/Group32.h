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

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group32Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
};

struct Group32Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
};

struct Group32Var3
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
  uint64_t time48;
};

struct Group32Var4
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
  uint64_t time48;
};

struct Group32Var5
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var5&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
};

struct Group32Var6
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var6&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
};

struct Group32Var7
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var7&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
  uint64_t time48;
};

struct Group32Var8
{
  static const GroupVariationID ID;
  static const size_t SIZE = 15;

  typedef Analog Target;
  static Analog Convert(openpal::ReadOnlyBuffer&);

  static Group32Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group32Var8&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
  uint64_t time48;
};


}

#endif
