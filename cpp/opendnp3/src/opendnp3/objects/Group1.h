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

#ifndef __OPENDNP3_GENERATED_GROUP1_H_
#define __OPENDNP3_GENERATED_GROUP1_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group1Var0
{
  static const GroupVariationID ID;
};

struct Group1Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 1;

  typedef Binary Target;
  static Binary ReadAndConvert(openpal::ReadOnlyBuffer&);
  static void ConvertAndWrite(const Binary&, openpal::WriteBuffer&);

  static Group1Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group1Var2&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group1Var1
{
  static const GroupVariationID ID;
};


}

#endif
