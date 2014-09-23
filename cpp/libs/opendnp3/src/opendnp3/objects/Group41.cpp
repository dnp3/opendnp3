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

#include "Group41.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group41Var1 Group41Var1::Read(ReadOnlyBuffer& buffer)
{
  Group41Var1 obj;
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  return obj;
}

void Group41Var1::Write(const Group41Var1& arg, openpal::WriteBuffer& buffer)
{
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
}


AnalogOutputInt32 Group41Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group41Var1::Read(buff);
  return AnalogOutputInt32Factory::From(gv.value, gv.status);
}

void Group41Var1::WriteTarget(const AnalogOutputInt32& value, openpal::WriteBuffer& buff)
{
  Group41Var1::Write(ConvertGroup41Var1::Apply(value), buff);
}


Group41Var2 Group41Var2::Read(ReadOnlyBuffer& buffer)
{
  Group41Var2 obj;
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  return obj;
}

void Group41Var2::Write(const Group41Var2& arg, openpal::WriteBuffer& buffer)
{
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
}


AnalogOutputInt16 Group41Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group41Var2::Read(buff);
  return AnalogOutputInt16Factory::From(gv.value, gv.status);
}

void Group41Var2::WriteTarget(const AnalogOutputInt16& value, openpal::WriteBuffer& buff)
{
  Group41Var2::Write(ConvertGroup41Var2::Apply(value), buff);
}


Group41Var3 Group41Var3::Read(ReadOnlyBuffer& buffer)
{
  Group41Var3 obj;
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  return obj;
}

void Group41Var3::Write(const Group41Var3& arg, openpal::WriteBuffer& buffer)
{
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
}


AnalogOutputFloat32 Group41Var3::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group41Var3::Read(buff);
  return AnalogOutputFloat32Factory::From(gv.value, gv.status);
}

void Group41Var3::WriteTarget(const AnalogOutputFloat32& value, openpal::WriteBuffer& buff)
{
  Group41Var3::Write(ConvertGroup41Var3::Apply(value), buff);
}


Group41Var4 Group41Var4::Read(ReadOnlyBuffer& buffer)
{
  Group41Var4 obj;
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  return obj;
}

void Group41Var4::Write(const Group41Var4& arg, openpal::WriteBuffer& buffer)
{
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
}


AnalogOutputDouble64 Group41Var4::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group41Var4::Read(buff);
  return AnalogOutputDouble64Factory::From(gv.value, gv.status);
}

void Group41Var4::WriteTarget(const AnalogOutputDouble64& value, openpal::WriteBuffer& buff)
{
  Group41Var4::Write(ConvertGroup41Var4::Apply(value), buff);
}



}
