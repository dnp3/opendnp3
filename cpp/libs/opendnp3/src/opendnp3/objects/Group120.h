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

#ifndef OPENDNP3_GROUP120_H
#define OPENDNP3_GROUP120_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3 {

struct Group120Var3
{
  static GroupVariationID ID() { return GroupVariationID(120,3); }
  static uint32_t Size() { return 6; }
  static bool Read(openpal::ReadBufferView&, Group120Var3&);
  static void Write(const Group120Var3&, openpal::WriteBufferView&);
  uint32_t challengeSeqNum;
  uint16_t userNum;
};

struct Group120Var4
{
  static GroupVariationID ID() { return GroupVariationID(120,4); }
  static uint32_t Size() { return 2; }
  static bool Read(openpal::ReadBufferView&, Group120Var4&);
  static void Write(const Group120Var4&, openpal::WriteBufferView&);
  uint16_t userNum;
};


}

#endif
