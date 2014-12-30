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

#ifndef OPENDNP3_GROUP13_H
#define OPENDNP3_GROUP13_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/BinaryCommandEvent.h"

namespace opendnp3 {

struct Group13Var1
{
  static GroupVariationID ID() { return GroupVariationID(13,1); }
  static uint32_t Size() { return 1; }
  static Group13Var1 Read(openpal::ReadBufferView&);
  static void Write(const Group13Var1&, openpal::WriteBufferView&);

  static DNP3Serializer<BinaryCommandEvent> Inst() { return DNP3Serializer<BinaryCommandEvent>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef BinaryCommandEvent Target;
  static BinaryCommandEvent ReadTarget(openpal::ReadBufferView&);
  static void WriteTarget(const BinaryCommandEvent&, openpal::WriteBufferView&);

  uint8_t flags;
};

struct Group13Var2
{
  static GroupVariationID ID() { return GroupVariationID(13,2); }
  static uint32_t Size() { return 7; }
  static Group13Var2 Read(openpal::ReadBufferView&);
  static void Write(const Group13Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<BinaryCommandEvent> Inst() { return DNP3Serializer<BinaryCommandEvent>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef BinaryCommandEvent Target;
  static BinaryCommandEvent ReadTarget(openpal::ReadBufferView&);
  static void WriteTarget(const BinaryCommandEvent&, openpal::WriteBufferView&);

  uint8_t flags;
  uint64_t time;
};


}

#endif
