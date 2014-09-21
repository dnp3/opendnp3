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

#include "Group20.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group20Var1 Group20Var1::Read(ReadOnlyBuffer& buffer)
{
  Group20Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var1::Write(const Group20Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


Counter Group20Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group20Var1::Read(buff);
  return CounterFactory::From(gv.flags, gv.value);
}

void Group20Var1::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group20Var1::Write(ConvertGroup20Var1::Apply(value), buff);
}


Group20Var2 Group20Var2::Read(ReadOnlyBuffer& buffer)
{
  Group20Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var2::Write(const Group20Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


Counter Group20Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group20Var2::Read(buff);
  return CounterFactory::From(gv.flags, gv.value);
}

void Group20Var2::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group20Var2::Write(ConvertGroup20Var2::Apply(value), buff);
}


Group20Var5 Group20Var5::Read(ReadOnlyBuffer& buffer)
{
  Group20Var5 obj;
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var5::Write(const Group20Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


Counter Group20Var5::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group20Var5::Read(buff);
  return CounterFactory::From(gv.value);
}

void Group20Var5::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group20Var5::Write(ConvertGroup20Var5::Apply(value), buff);
}


Group20Var6 Group20Var6::Read(ReadOnlyBuffer& buffer)
{
  Group20Var6 obj;
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var6::Write(const Group20Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


Counter Group20Var6::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group20Var6::Read(buff);
  return CounterFactory::From(gv.value);
}

void Group20Var6::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group20Var6::Write(ConvertGroup20Var6::Apply(value), buff);
}



}
