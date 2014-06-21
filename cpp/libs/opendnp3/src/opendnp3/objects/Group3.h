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

#ifndef __OPENDNP3_GENERATED_GROUP3_H_
#define __OPENDNP3_GENERATED_GROUP3_H_

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/IDNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group3Var0
{
  static const GroupVariationID ID;
};

struct Group3Var1
{
  static const GroupVariationID ID;
};

struct Group3Var2
{
  static const GroupVariationID ID;
  typedef DoubleBitBinary Target;
  static const uint32_t SIZE = 1;
  static Group3Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group3Var2&, openpal::WriteBuffer&);

  uint8_t flags;
};

struct Group3Var2Serializer : public IDNP3Serializer<DoubleBitBinary>
{

  static IDNP3Serializer<DoubleBitBinary>* Inst() { return &mInstance; }

  GroupVariationID ID() const { return Group3Var2::ID; }

  uint32_t Size() const { return Group3Var2::SIZE; }

  typedef DoubleBitBinary Target;
  DoubleBitBinary Read(openpal::ReadOnlyBuffer&) const;
  void Write(const DoubleBitBinary&, openpal::WriteBuffer&) const;

  private:

  static Group3Var2Serializer mInstance;
};


}

#endif
