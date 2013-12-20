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

#include "Group21.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group21Var1::ID(21,1);

Group21Var1 Group21Var1::Read(ReadOnlyBuffer& buffer)
{
  Group21Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var1::Write(const Group21Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

const GroupVariationID  Group21Var2::ID(21,2);

Group21Var2 Group21Var2::Read(ReadOnlyBuffer& buffer)
{
  Group21Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var2::Write(const Group21Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

const GroupVariationID  Group21Var3::ID(21,3);

Group21Var3 Group21Var3::Read(ReadOnlyBuffer& buffer)
{
  Group21Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var3::Write(const Group21Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

const GroupVariationID  Group21Var4::ID(21,4);

Group21Var4 Group21Var4::Read(ReadOnlyBuffer& buffer)
{
  Group21Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var4::Write(const Group21Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

const GroupVariationID  Group21Var5::ID(21,5);

Group21Var5 Group21Var5::Read(ReadOnlyBuffer& buffer)
{
  Group21Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var5::Write(const Group21Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

const GroupVariationID  Group21Var6::ID(21,6);

Group21Var6 Group21Var6::Read(ReadOnlyBuffer& buffer)
{
  Group21Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var6::Write(const Group21Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

const GroupVariationID  Group21Var7::ID(21,7);

Group21Var7 Group21Var7::Read(ReadOnlyBuffer& buffer)
{
  Group21Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var7::Write(const Group21Var7& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

const GroupVariationID  Group21Var8::ID(21,8);

Group21Var8 Group21Var8::Read(ReadOnlyBuffer& buffer)
{
  Group21Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group21Var8::Write(const Group21Var8& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

const GroupVariationID  Group21Var9::ID(21,9);

Group21Var9 Group21Var9::Read(ReadOnlyBuffer& buffer)
{
  Group21Var9 obj;
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var9::Write(const Group21Var9& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

const GroupVariationID  Group21Var10::ID(21,10);

Group21Var10 Group21Var10::Read(ReadOnlyBuffer& buffer)
{
  Group21Var10 obj;
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var10::Write(const Group21Var10& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

const GroupVariationID  Group21Var11::ID(21,11);

Group21Var11 Group21Var11::Read(ReadOnlyBuffer& buffer)
{
  Group21Var11 obj;
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group21Var11::Write(const Group21Var11& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

const GroupVariationID  Group21Var12::ID(21,12);

Group21Var12 Group21Var12::Read(ReadOnlyBuffer& buffer)
{
  Group21Var12 obj;
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group21Var12::Write(const Group21Var12& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}


}
