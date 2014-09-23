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

#ifndef OPENDNP3_GROUP1_H
#define OPENDNP3_GROUP1_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group1Var0
{
  static GroupVariationID ID() { return GroupVariationID(1,0); }
};

struct Group1Var2
{
  static GroupVariationID ID() { return GroupVariationID(1,2); }
  static uint32_t Size() { return 1; }
  static Group1Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group1Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<Binary> Inst() { return DNP3Serializer<Binary>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Binary Target;
  static Binary ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Binary&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group1Var1
{
  static GroupVariationID ID() { return GroupVariationID(1,1); }
};


}

#endif
