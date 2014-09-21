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

#include "Group12.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group12Var1 Group12Var1::Read(ReadOnlyBuffer& buffer)
{
  Group12Var1 obj;
  obj.code = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.onTime = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.offTime = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(UInt8::Read(buffer));
  buffer.Advance(1);
  return obj;
}

void Group12Var1::Write(const Group12Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.code);
  buffer.Advance(1);
  UInt8::Write(buffer, arg.count);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.onTime);
  buffer.Advance(4);
  UInt32::Write(buffer, arg.offTime);
  buffer.Advance(4);
  UInt8::Write(buffer, CommandStatusToType(arg.status));
  buffer.Advance(1);
}


ControlRelayOutputBlock Group12Var1::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group12Var1::Read(buff);
  return ControlRelayOutputBlockFactory::From(gv.code, gv.count, gv.onTime, gv.offTime, gv.status);
}

void Group12Var1::WriteTarget(const ControlRelayOutputBlock& value, openpal::WriteBuffer& buff)
{
  Group12Var1::Write(ConvertGroup12Var1::Apply(value), buff);
}



}
