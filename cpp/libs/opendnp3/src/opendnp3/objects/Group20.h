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

#ifndef OPENDNP3_GROUP20_H
#define OPENDNP3_GROUP20_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/container/WriteBuffer.h>
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group20Var1
{
  static GroupVariationID ID() { return GroupVariationID(20,1); }
  static uint32_t Size() { return 5; }
  static Group20Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var1&, openpal::WriteBuffer&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static Counter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Counter&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint8_t flags;
  uint32_t value;
};

struct Group20Var2
{
  static GroupVariationID ID() { return GroupVariationID(20,2); }
  static uint32_t Size() { return 3; }
  static Group20Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var2&, openpal::WriteBuffer&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static Counter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Counter&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint8_t flags;
  uint16_t value;
};

struct Group20Var5
{
  static GroupVariationID ID() { return GroupVariationID(20,5); }
  static uint32_t Size() { return 4; }
  static Group20Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var5&, openpal::WriteBuffer&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static Counter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Counter&, openpal::WriteBuffer&);

  typedef uint32_t ValueType;
  uint32_t value;
};

struct Group20Var6
{
  static GroupVariationID ID() { return GroupVariationID(20,6); }
  static uint32_t Size() { return 2; }
  static Group20Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var6&, openpal::WriteBuffer&);

  static DNP3Serializer<Counter> Inst() { return DNP3Serializer<Counter>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Counter Target;
  static Counter ReadTarget(openpal::ReadOnlyBuffer&);
  static void WriteTarget(const Counter&, openpal::WriteBuffer&);

  typedef uint16_t ValueType;
  uint16_t value;
};


}

#endif
