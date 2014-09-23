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

#include "Group22.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group22Var1 Group22Var1::Read(ReadOnlyBuffer& buffer)
{
  Group22Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group22Var1::Write(const Group22Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


Counter Group22Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group22Var1::Read(buff);
  return CounterFactory::From(gv.flags, gv.value);
}

void Group22Var1::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group22Var1::Write(ConvertGroup22Var1::Apply(value), buff);
}


Group22Var2 Group22Var2::Read(ReadOnlyBuffer& buffer)
{
  Group22Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group22Var2::Write(const Group22Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


Counter Group22Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group22Var2::Read(buff);
  return CounterFactory::From(gv.flags, gv.value);
}

void Group22Var2::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group22Var2::Write(ConvertGroup22Var2::Apply(value), buff);
}


Group22Var5 Group22Var5::Read(ReadOnlyBuffer& buffer)
{
  Group22Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var5::Write(const Group22Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


Counter Group22Var5::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group22Var5::Read(buff);
  return CounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group22Var5::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group22Var5::Write(ConvertGroup22Var5::Apply(value), buff);
}


Group22Var6 Group22Var6::Read(ReadOnlyBuffer& buffer)
{
  Group22Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var6::Write(const Group22Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


Counter Group22Var6::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group22Var6::Read(buff);
  return CounterFactory::From(gv.flags, gv.value, gv.time);
}

void Group22Var6::WriteTarget(const Counter& value, openpal::WriteBuffer& buff)
{
  Group22Var6::Write(ConvertGroup22Var6::Apply(value), buff);
}



}
