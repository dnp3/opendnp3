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

#include "Group23.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group23Var1::ID(23,1);

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

const GroupVariationID  Group23Var2::ID(23,2);

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

const GroupVariationID  Group23Var5::ID(23,5);

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

const GroupVariationID  Group23Var6::ID(23,6);

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


}
