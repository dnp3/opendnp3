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

#include "Group23.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group23Var1 Group23Var1::Read(ReadOnlyBuffer& buffer)
{
  Group23Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group23Var1::Write(const Group23Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


FrozenCounter Group23Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group23Var1::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value);
}

void Group23Var1::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group23Var1::Write(ConvertGroup23Var1::Apply(value), buff);
}


Group23Var2 Group23Var2::Read(ReadOnlyBuffer& buffer)
{
  Group23Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group23Var2::Write(const Group23Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


FrozenCounter Group23Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group23Var2::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value);
}

void Group23Var2::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group23Var2::Write(ConvertGroup23Var2::Apply(value), buff);
}


Group23Var5 Group23Var5::Read(ReadOnlyBuffer& buffer)
{
  Group23Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group23Var5::Write(const Group23Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


FrozenCounter Group23Var5::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group23Var5::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group23Var5::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group23Var5::Write(ConvertGroup23Var5::Apply(value), buff);
}


Group23Var6 Group23Var6::Read(ReadOnlyBuffer& buffer)
{
  Group23Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group23Var6::Write(const Group23Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


FrozenCounter Group23Var6::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group23Var6::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group23Var6::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group23Var6::Write(ConvertGroup23Var6::Apply(value), buff);
}



}
