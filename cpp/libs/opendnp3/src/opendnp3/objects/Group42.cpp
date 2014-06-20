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

#include "Group42.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group42Var1::ID(42,1);

Group42Var1 Group42Var1::Read(ReadOnlyBuffer& buffer)
{
  Group42Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group42Var1::Write(const Group42Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}

Group42Var1Serializer Group42Var1Serializer::mInstance;

AnalogOutputStatus Group42Var1Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var1::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group42Var1Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var1::Write(ConvertGroup42Var1::Apply(value), buff);
}


const GroupVariationID  Group42Var2::ID(42,2);

Group42Var2 Group42Var2::Read(ReadOnlyBuffer& buffer)
{
  Group42Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group42Var2::Write(const Group42Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}

Group42Var2Serializer Group42Var2Serializer::mInstance;

AnalogOutputStatus Group42Var2Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var2::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group42Var2Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var2::Write(ConvertGroup42Var2::Apply(value), buff);
}


const GroupVariationID  Group42Var3::ID(42,3);

Group42Var3 Group42Var3::Read(ReadOnlyBuffer& buffer)
{
  Group42Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group42Var3::Write(const Group42Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Group42Var3Serializer Group42Var3Serializer::mInstance;

AnalogOutputStatus Group42Var3Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var3::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value, gv.time);
}

void Group42Var3Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var3::Write(ConvertGroup42Var3::Apply(value), buff);
}


const GroupVariationID  Group42Var4::ID(42,4);

Group42Var4 Group42Var4::Read(ReadOnlyBuffer& buffer)
{
  Group42Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group42Var4::Write(const Group42Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Group42Var4Serializer Group42Var4Serializer::mInstance;

AnalogOutputStatus Group42Var4Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var4::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value, gv.time);
}

void Group42Var4Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var4::Write(ConvertGroup42Var4::Apply(value), buff);
}


const GroupVariationID  Group42Var5::ID(42,5);

Group42Var5 Group42Var5::Read(ReadOnlyBuffer& buffer)
{
  Group42Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group42Var5::Write(const Group42Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}

Group42Var5Serializer Group42Var5Serializer::mInstance;

AnalogOutputStatus Group42Var5Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var5::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group42Var5Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var5::Write(ConvertGroup42Var5::Apply(value), buff);
}


const GroupVariationID  Group42Var6::ID(42,6);

Group42Var6 Group42Var6::Read(ReadOnlyBuffer& buffer)
{
  Group42Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  return obj;
}

void Group42Var6::Write(const Group42Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}

Group42Var6Serializer Group42Var6Serializer::mInstance;

AnalogOutputStatus Group42Var6Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var6::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value);
}

void Group42Var6Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var6::Write(ConvertGroup42Var6::Apply(value), buff);
}


const GroupVariationID  Group42Var7::ID(42,7);

Group42Var7 Group42Var7::Read(ReadOnlyBuffer& buffer)
{
  Group42Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group42Var7::Write(const Group42Var7& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Group42Var7Serializer Group42Var7Serializer::mInstance;

AnalogOutputStatus Group42Var7Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var7::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value, gv.time);
}

void Group42Var7Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var7::Write(ConvertGroup42Var7::Apply(value), buff);
}


const GroupVariationID  Group42Var8::ID(42,8);

Group42Var8 Group42Var8::Read(ReadOnlyBuffer& buffer)
{
  Group42Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = DoubleFloat::Read(buffer);
  buffer.Advance(8);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group42Var8::Write(const Group42Var8& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Group42Var8Serializer Group42Var8Serializer::mInstance;

AnalogOutputStatus Group42Var8Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group42Var8::Read(buff);
  return AnalogOutputStatusFactory::From(gv.flags, gv.value, gv.time);
}

void Group42Var8Serializer::Write(const AnalogOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group42Var8::Write(ConvertGroup42Var8::Apply(value), buff);
}



}
