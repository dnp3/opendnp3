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

#ifndef __OPENDNP3_GENERATED_GROUP10_H_
#define __OPENDNP3_GENERATED_GROUP10_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group10Var0
{
  static const GroupVariationID ID;
};

struct Group10Var1
{
  static const GroupVariationID ID;
};

struct Group10Var2
{
  static const GroupVariationID ID;
  static const uint32_t SIZE = 1;
  static Group10Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group10Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(ID, SIZE, &ReadTarget, &WriteTarget); }

  typedef BinaryOutputStatus Target;
  static BinaryOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const BinaryOutputStatus&, openpal::WriteBuffer&);

  uint8_t flags;
};


}

#endif
