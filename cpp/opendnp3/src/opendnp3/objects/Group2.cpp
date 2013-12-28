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

#include "Group2.h"

#include "MeasurementFactory.h"
#include "../WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group2Var0::ID(2,0);

const GroupVariationID  Group2Var1::ID(2,1);

Group2Var1 Group2Var1::Read(ReadOnlyBuffer& buffer)
{
  Group2Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

void Group2Var1::Write(const Group2Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}

Binary Group2Var1::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return BinaryFactory::From(gv.flags);
}

void Group2Var1::ConvertAndWrite(const Binary& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup2Var1::Apply(value), buff);
}


const GroupVariationID  Group2Var2::ID(2,2);

Group2Var2 Group2Var2::Read(ReadOnlyBuffer& buffer)
{
  Group2Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group2Var2::Write(const Group2Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}

Binary Group2Var2::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return BinaryFactory::From(gv.flags, gv.time);
}

void Group2Var2::ConvertAndWrite(const Binary& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup2Var2::Apply(value), buff);
}


const GroupVariationID  Group2Var3::ID(2,3);

Group2Var3 Group2Var3::Read(ReadOnlyBuffer& buffer)
{
  Group2Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group2Var3::Write(const Group2Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.time);
  buffer.Advance(2);
}

Binary Group2Var3::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return BinaryFactory::From(gv.flags, gv.time);
}

void Group2Var3::ConvertAndWrite(const Binary& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup2Var3::Apply(value), buff);
}



}
