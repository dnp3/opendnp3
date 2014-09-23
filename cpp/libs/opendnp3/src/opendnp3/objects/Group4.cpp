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

#include "Group4.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group4Var1 Group4Var1::Read(ReadOnlyBuffer& buffer)
{
  Group4Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

void Group4Var1::Write(const Group4Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}


DoubleBitBinary Group4Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group4Var1::Read(buff);
  return DoubleBitBinaryFactory::From(gv.flags);
}

void Group4Var1::WriteTarget(const DoubleBitBinary& value, openpal::WriteBuffer& buff)
{
  Group4Var1::Write(ConvertGroup4Var1::Apply(value), buff);
}


Group4Var2 Group4Var2::Read(ReadOnlyBuffer& buffer)
{
  Group4Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group4Var2::Write(const Group4Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


DoubleBitBinary Group4Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group4Var2::Read(buff);
  return DoubleBitBinaryFactory::From(gv.flags, gv.time);
}

void Group4Var2::WriteTarget(const DoubleBitBinary& value, openpal::WriteBuffer& buff)
{
  Group4Var2::Write(ConvertGroup4Var2::Apply(value), buff);
}


Group4Var3 Group4Var3::Read(ReadOnlyBuffer& buffer)
{
  Group4Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group4Var3::Write(const Group4Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.time);
  buffer.Advance(2);
}


DoubleBitBinary Group4Var3::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group4Var3::Read(buff);
  return DoubleBitBinaryFactory::From(gv.flags, gv.time);
}

void Group4Var3::WriteTarget(const DoubleBitBinary& value, openpal::WriteBuffer& buff)
{
  Group4Var3::Write(ConvertGroup4Var3::Apply(value), buff);
}



}
