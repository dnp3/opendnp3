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

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"

namespace opendnp3 {

struct Group120Var1
{
  static GroupVariationID ID() { return GroupVariationID(120,1); }
};

struct Group120Var2
{
  static GroupVariationID ID() { return GroupVariationID(120,2); }
};

struct Group120Var3
{
  static GroupVariationID ID() { return GroupVariationID(120,3); }
  static uint32_t Size() { return 6; }
  static bool Read(openpal::ReadBufferView&, Group120Var3&);
  static bool Write(const Group120Var3&, openpal::WriteBufferView&);
  uint32_t challengeSeqNum;
  uint16_t userNum;
};

struct Group120Var4
{
  static GroupVariationID ID() { return GroupVariationID(120,4); }
  static uint32_t Size() { return 2; }
  static bool Read(openpal::ReadBufferView&, Group120Var4&);
  static bool Write(const Group120Var4&, openpal::WriteBufferView&);
  uint16_t userNum;
};

struct Group120Var5
{
  static GroupVariationID ID() { return GroupVariationID(120,5); }
};

struct Group120Var6
{
  static GroupVariationID ID() { return GroupVariationID(120,6); }
};

struct Group120Var7
{
  static GroupVariationID ID() { return GroupVariationID(120,7); }
};

struct Group120Var8
{
  static GroupVariationID ID() { return GroupVariationID(120,8); }
};

struct Group120Var9
{
  static GroupVariationID ID() { return GroupVariationID(120,9); }
};

struct Group120Var10
{
  static GroupVariationID ID() { return GroupVariationID(120,10); }
};

struct Group120Var11
{
  static GroupVariationID ID() { return GroupVariationID(120,11); }
};

struct Group120Var12
{
  static GroupVariationID ID() { return GroupVariationID(120,12); }
};

struct Group120Var13
{
  static GroupVariationID ID() { return GroupVariationID(120,13); }
};

struct Group120Var14
{
  static GroupVariationID ID() { return GroupVariationID(120,14); }
};

struct Group120Var15
{
  static GroupVariationID ID() { return GroupVariationID(120,15); }
};


}

#endif
