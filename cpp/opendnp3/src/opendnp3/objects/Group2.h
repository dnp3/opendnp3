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

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group2Var0
{
  static const GroupVariationID ID;
};

struct Group2Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 1;

  typedef Binary Target;
  static Binary Convert(openpal::ReadOnlyBuffer&);

  static Group2Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var1&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group2Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 7;

  typedef Binary Target;
  static Binary Convert(openpal::ReadOnlyBuffer&);

  static Group2Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time48;
};

struct Group2Var3
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;

  typedef Binary Target;
  static Binary Convert(openpal::ReadOnlyBuffer&);

  static Group2Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group2Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t time16;
};


}

#endif
