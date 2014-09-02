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

#ifndef __OPENDNP3_GENERATED_GROUP11_H_
#define __OPENDNP3_GENERATED_GROUP11_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group11Var1
{
  static const GroupVariationID ID;
  typedef BinaryOutputStatus Target;
  static const uint32_t SIZE = 1;
  static Group11Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var1&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group11Var1Serializer
{

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(Group11Var1::ID, Group11Var1::SIZE, &Read, &Write); }

  typedef BinaryOutputStatus Target;
  static BinaryOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const BinaryOutputStatus&, openpal::WriteBuffer&);

};

struct Group11Var2
{
  static const GroupVariationID ID;
  typedef BinaryOutputStatus Target;
  static const uint32_t SIZE = 7;
  static Group11Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group11Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint64_t time;
};

struct Group11Var2Serializer
{

  static DNP3Serializer<BinaryOutputStatus> Inst() { return DNP3Serializer<BinaryOutputStatus>(Group11Var2::ID, Group11Var2::SIZE, &Read, &Write); }

  typedef BinaryOutputStatus Target;
  static BinaryOutputStatus Read(openpal::ReadOnlyBuffer&);
  static void Write(const BinaryOutputStatus&, openpal::WriteBuffer&);

};


}

#endif
