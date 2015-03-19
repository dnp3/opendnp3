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

#ifndef OPENDNP3_GROUP32_H
#define OPENDNP3_GROUP32_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/GroupVariationID.h"
#include "opendnp3/app/DNP3Serializer.h"
#include "opendnp3/app/MeasurementTypes.h"

namespace opendnp3 {

struct Group32Var0
{
  static GroupVariationID ID() { return GroupVariationID(32,0); }
};

struct Group32Var1
{
  static GroupVariationID ID() { return GroupVariationID(32,1); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group32Var1&);
  static bool Write(const Group32Var1&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
};

struct Group32Var2
{
  static GroupVariationID ID() { return GroupVariationID(32,2); }
  static uint32_t Size() { return 3; }
  static bool Read(openpal::ReadBufferView&, Group32Var2&);
  static bool Write(const Group32Var2&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
};

struct Group32Var3
{
  static GroupVariationID ID() { return GroupVariationID(32,3); }
  static uint32_t Size() { return 11; }
  static bool Read(openpal::ReadBufferView&, Group32Var3&);
  static bool Write(const Group32Var3&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int32_t ValueType;
  uint8_t flags;
  int32_t value;
  DNPTime time;
};

struct Group32Var4
{
  static GroupVariationID ID() { return GroupVariationID(32,4); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group32Var4&);
  static bool Write(const Group32Var4&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef int16_t ValueType;
  uint8_t flags;
  int16_t value;
  DNPTime time;
};

struct Group32Var5
{
  static GroupVariationID ID() { return GroupVariationID(32,5); }
  static uint32_t Size() { return 5; }
  static bool Read(openpal::ReadBufferView&, Group32Var5&);
  static bool Write(const Group32Var5&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef float ValueType;
  uint8_t flags;
  float value;
};

struct Group32Var6
{
  static GroupVariationID ID() { return GroupVariationID(32,6); }
  static uint32_t Size() { return 9; }
  static bool Read(openpal::ReadBufferView&, Group32Var6&);
  static bool Write(const Group32Var6&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef double ValueType;
  uint8_t flags;
  double value;
};

struct Group32Var7
{
  static GroupVariationID ID() { return GroupVariationID(32,7); }
  static uint32_t Size() { return 11; }
  static bool Read(openpal::ReadBufferView&, Group32Var7&);
  static bool Write(const Group32Var7&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef float ValueType;
  uint8_t flags;
  float value;
  DNPTime time;
};

struct Group32Var8
{
  static GroupVariationID ID() { return GroupVariationID(32,8); }
  static uint32_t Size() { return 15; }
  static bool Read(openpal::ReadBufferView&, Group32Var8&);
  static bool Write(const Group32Var8&, openpal::WriteBufferView&);

  static DNP3Serializer<Analog> Inst() { return DNP3Serializer<Analog>(ID(), Size(), &ReadTarget, &WriteTarget); }

  typedef Analog Target;
  static bool ReadTarget(openpal::ReadBufferView&, Analog&);
  static bool WriteTarget(const Analog&, openpal::WriteBufferView&);

  typedef double ValueType;
  uint8_t flags;
  double value;
  DNPTime time;
};


}

#endif
