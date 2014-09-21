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

#ifndef OPENDNP3_GROUP11_H
#define OPENDNP3_GROUP11_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group11Var1
{
  static GroupVariationID ID() { return GroupVariationID(11,1); }
  static uint32_t Size() { return 1; }
  static Group11Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef BinaryOutputStatus Target;
  static BinaryOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const BinaryOutputStatus&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group11Var2
{
  static GroupVariationID ID() { return GroupVariationID(11,2); }
  static uint32_t Size() { return 7; }
  static Group11Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef BinaryOutputStatus Target;
  static BinaryOutputStatus ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const BinaryOutputStatus&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time;
};


}

#endif
