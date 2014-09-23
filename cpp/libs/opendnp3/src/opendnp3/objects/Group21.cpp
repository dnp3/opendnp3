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

#include "Group21.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group21Var1 Group21Var1::Read(ReadOnlyBuffer& buffer)
{
  Group21Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var1::Write(const Group21Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


FrozenCounter Group21Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var1::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value);
}

void Group21Var1::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var1::Write(ConvertGroup21Var1::Apply(value), buff);
}


Group21Var2 Group21Var2::Read(ReadOnlyBuffer& buffer)
{
  Group21Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var2::Write(const Group21Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


FrozenCounter Group21Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var2::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value);
}

void Group21Var2::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var2::Write(ConvertGroup21Var2::Apply(value), buff);
}


Group21Var5 Group21Var5::Read(ReadOnlyBuffer& buffer)
{
  Group21Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var5::Write(const Group21Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


FrozenCounter Group21Var5::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var5::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group21Var5::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var5::Write(ConvertGroup21Var5::Apply(value), buff);
}


Group21Var6 Group21Var6::Read(ReadOnlyBuffer& buffer)
{
  Group21Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var6::Write(const Group21Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


FrozenCounter Group21Var6::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var6::Read(buff);
  return FrozenCounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group21Var6::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var6::Write(ConvertGroup21Var6::Apply(value), buff);
}


Group21Var9 Group21Var9::Read(ReadOnlyBuffer& buffer)
{
  Group21Var9 obj;
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var9::Write(const Group21Var9& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


FrozenCounter Group21Var9::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var9::Read(buff);
  return FrozenCounterFactory::From(gv.value);
}

void Group21Var9::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var9::Write(ConvertGroup21Var9::Apply(value), buff);
}


Group21Var10 Group21Var10::Read(ReadOnlyBuffer& buffer)
{
  Group21Var10 obj;
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var10::Write(const Group21Var10& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


FrozenCounter Group21Var10::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group21Var10::Read(buff);
  return FrozenCounterFactory::From(gv.value);
}

void Group21Var10::WriteTarget(const FrozenCounter& value, openpal::WriteBuffer& buff)
{
  Group21Var10::Write(ConvertGroup21Var10::Apply(value), buff);
}



}
