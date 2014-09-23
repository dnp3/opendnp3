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

#ifndef OPENDNP3_GROUP4_H
#define OPENDNP3_GROUP4_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group4Var1
{
  static GroupVariationID ID() { return GroupVariationID(4,1); }
  static uint32_t Size() { return 1; }
  static Group4Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group4Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<DoubleBitBinary> Inst() { return DNP3Serializer<DoubleBitBinary>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef DoubleBitBinary Target;
  static DoubleBitBinary ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const DoubleBitBinary&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group4Var2
{
  static GroupVariationID ID() { return GroupVariationID(4,2); }
  static uint32_t Size() { return 7; }
  static Group4Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group4Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<DoubleBitBinary> Inst() { return DNP3Serializer<DoubleBitBinary>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef DoubleBitBinary Target;
  static DoubleBitBinary ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const DoubleBitBinary&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time;
};

struct Group4Var3
{
  static GroupVariationID ID() { return GroupVariationID(4,3); }
  static uint32_t Size() { return 3; }
  static Group4Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group4Var3&, openpal::WriteBuffer&);

  static DNP3Serializer<DoubleBitBinary> Inst() { return DNP3Serializer<DoubleBitBinary>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef DoubleBitBinary Target;
  static DoubleBitBinary ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const DoubleBitBinary&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t time;
};


}

#endif
