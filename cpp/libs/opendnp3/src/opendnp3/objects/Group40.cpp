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

#include "Group40.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group40Var1 Group40Var1::Read(ReadOnlyBuffer& buffer)
{
  Group40Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group40Var1::Write(const Group40Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}


AnalogOutputStatus Group40Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group40Var1::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group40Var1::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group40Var1::Write(ConvertGroup40Var1::Apply(value), buff);
}


Group40Var2 Group40Var2::Read(ReadOnlyBuffer& buffer)
{
  Group40Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group40Var2::Write(const Group40Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}


AnalogOutputStatus Group40Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group40Var2::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group40Var2::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group40Var2::Write(ConvertGroup40Var2::Apply(value), buff);
}


Group40Var3 Group40Var3::Read(ReadOnlyBuffer& buffer)
{
  Group40Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group40Var3::Write(const Group40Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}


AnalogOutputStatus Group40Var3::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group40Var3::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group40Var3::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group40Var3::Write(ConvertGroup40Var3::Apply(value), buff);
}


Group40Var4 Group40Var4::Read(ReadOnlyBuffer& buffer)
{
  Group40Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  return obj;
}

void Group40Var4::Write(const Group40Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}


AnalogOutputStatus Group40Var4::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group40Var4::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group40Var4::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group40Var4::Write(ConvertGroup40Var4::Apply(value), buff);
}



}
