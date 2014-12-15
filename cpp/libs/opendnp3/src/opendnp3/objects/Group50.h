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

#ifndef OPENDNP3_GROUP50_H
#define OPENDNP3_GROUP50_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/TimeAndInterval.h"

namespace opendnp3 {

struct Group50Var1
{
  static GroupVariationID ID() { return GroupVariationID(50,1); }
  static uint32_t Size() { return 6; }
  static Group50Var1 Read(openpal::ReadBufferView&);
  static void Write(const Group50Var1&, openpal::WriteBufferView&);
  uint64_t time;
};

struct Group50Var4
{
  static GroupVariationID ID() { return GroupVariationID(50,4); }
  static uint32_t Size() { return 11; }
  static Group50Var4 Read(openpal::ReadBufferView&);
  static void Write(const Group50Var4&, openpal::WriteBufferView&);

  static DNP3Serializer<TimeAndInterval> Inst() { return DNP3Serializer<TimeAndInterval>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef TimeAndInterval Target;
  static TimeAndInterval ReadTarget(openpal::ReadBufferView&);
  static void WriteTarget(const TimeAndInterval&, openpal::WriteBufferView&);

  uint64_t time;
  uint32_t interval;
  uint8_t units;
};


}

#endif
