//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group30.h"

#include "MeasurementFactory.h"
#include "../WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group30Var1::ID(30,1);

Group30Var1 Group30Var1::Read(ReadOnlyBuffer& buffer)
{
  Group30Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group30Var1::Write(const Group30Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}

Analog Group30Var1::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group30Var1::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var1::Apply(value), buff);
}


const GroupVariationID  Group30Var2::ID(30,2);

Group30Var2 Group30Var2::Read(ReadOnlyBuffer& buffer)
{
  Group30Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group30Var2::Write(const Group30Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}

Analog Group30Var2::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group30Var2::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var2::Apply(value), buff);
}


const GroupVariationID  Group30Var3::ID(30,3);

Group30Var3 Group30Var3::Read(ReadOnlyBuffer& buffer)
{
  Group30Var3 obj;
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group30Var3::Write(const Group30Var3& arg, openpal::WriteBuffer& buffer)
{
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}

Analog Group30Var3::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.value);
}

void Group30Var3::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var3::Apply(value), buff);
}


const GroupVariationID  Group30Var4::ID(30,4);

Group30Var4 Group30Var4::Read(ReadOnlyBuffer& buffer)
{
  Group30Var4 obj;
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group30Var4::Write(const Group30Var4& arg, openpal::WriteBuffer& buffer)
{
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}

Analog Group30Var4::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.value);
}

void Group30Var4::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var4::Apply(value), buff);
}


const GroupVariationID  Group30Var5::ID(30,5);

Group30Var5 Group30Var5::Read(ReadOnlyBuffer& buffer)
{
  Group30Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group30Var5::Write(const Group30Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}

Analog Group30Var5::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group30Var5::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var5::Apply(value), buff);
}


const GroupVariationID  Group30Var6::ID(30,6);

Group30Var6 Group30Var6::Read(ReadOnlyBuffer& buffer)
{
  Group30Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  return obj;
}

void Group30Var6::Write(const Group30Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}

Analog Group30Var6::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return AnalogFactory::From(gv.flags, gv.value);
}

void Group30Var6::ConvertAndWrite(const Analog& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup30Var6::Apply(value), buff);
}



}
