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

#include "Group11.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group11Var1 Group11Var1::Read(ReadOnlyBuffer& buffer)
{
  Group11Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

void Group11Var1::Write(const Group11Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}


BinaryOutputStatus Group11Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group11Var1::Read(buff);
  return BinaryOutputStatusFactory::From(gv.flags);
}

void Group11Var1::WriteTarget(const BinaryOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group11Var1::Write(ConvertGroup11Var1::Apply(value), buff);
}


Group11Var2 Group11Var2::Read(ReadOnlyBuffer& buffer)
{
  Group11Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group11Var2::Write(const Group11Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


BinaryOutputStatus Group11Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group11Var2::Read(buff);
  return BinaryOutputStatusFactory::From(gv.flags, gv.time);
}

void Group11Var2::WriteTarget(const BinaryOutputStatus& value, openpal::WriteBuffer& buff)
{
  Group11Var2::Write(ConvertGroup11Var2::Apply(value), buff);
}



}
