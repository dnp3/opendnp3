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

#ifndef __OPENDNP3_GENERATED_GROUP12_H_
#define __OPENDNP3_GENERATED_GROUP12_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/gen/ControlCode.h>
#include <opendnp3/gen/CommandStatus.h>
#include <opendnp3/ControlRelayOutputBlock.h>

namespace opendnp3 {

struct Group12Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 11;

  typedef ControlRelayOutputBlock Target;
  static ControlRelayOutputBlock Convert(openpal::ReadOnlyBuffer&);

  static Group12Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group12Var1&, openpal::WriteBuffer&);

  ControlCode code;
  uint8_t count;
  uint32_t onTime;
  uint32_t offTime;
  CommandStatus status;
};


}

#endif
