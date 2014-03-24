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

#include <openpal/Serialization.h>

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


}
