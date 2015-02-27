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

#ifndef OPENDNP3_GROUP22_H
#define OPENDNP3_GROUP22_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group22Var1
{
  static GroupVariationID ID() { return GroupVariationID(22,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group22Var1&);
  static bool Write(const Group22Var1&, openpal::WriteBufferView&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static bool ReadTarget(openpal::ReadBufferView&, Counter&);
  static bool WriteTarget(const Counter&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group22Var2
{
  static GroupVariationID ID() { return GroupVariationID(22,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group22Var2&);
  static bool Write(const Group22Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static bool ReadTarget(openpal::ReadBufferView&, Counter&);
  static bool WriteTarget(const Counter&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group22Var5
{
  static GroupVariationID ID() { return GroupVariationID(22,5); }
  static uint32_t Size() { return 11; }
  static bool Read(openpal::ReadBufferView&, Group22Var5&);
  static bool Write(const Group22Var5&, openpal::WriteBufferView&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static bool ReadTarget(openpal::ReadBufferView&, Counter&);
  static bool WriteTarget(const Counter&, openpal::WriteBufferView&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
  DNPTime time;
};

struct Group22Var6
{
  static GroupVariationID ID() { return GroupVariationID(22,6); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group22Var6&);
  static bool Write(const Group22Var6&, openpal::WriteBufferView&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static bool ReadTarget(openpal::ReadBufferView&, Counter&);
  static bool WriteTarget(const Counter&, openpal::WriteBufferView&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
  DNPTime time;
};


}

#endif
