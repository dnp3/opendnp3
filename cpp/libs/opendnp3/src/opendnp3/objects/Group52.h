//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef OPENDNP3_GROUP52_H
#define OPENDNP3_GROUP52_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3 {

struct Group52Var1
{
  static GroupVariationID ID() { return GroupVariationID(52,1); }
  static uint32_t Size() { return 2; }
  static Group52Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group52Var1&, openpal::WriteBuffer&);
  uint16_t time;
};

struct Group52Var2
{
  static GroupVariationID ID() { return GroupVariationID(52,2); }
  static uint32_t Size() { return 2; }
  static Group52Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group52Var2&, openpal::WriteBuffer&);
  uint16_t time;
};


}

#endif
