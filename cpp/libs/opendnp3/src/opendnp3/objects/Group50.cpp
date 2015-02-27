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

#include "Group50.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group50Var1::Read(ReadBufferView& buffer, Group50Var1& output)
{
  return Parse::Many(buffer, output.time);
}

void Group50Var1::Write(const Group50Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group50Var4::Read(ReadBufferView& buffer, Group50Var4& output)
{
  return Parse::Many(buffer, output.time, output.interval, output.units);
}

void Group50Var4::Write(const Group50Var4& arg, openpal::WriteBufferView& buffer)
{
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
  UInt32::Write(buffer, arg.interval);
  buffer.Advance(4);
  UInt8::Write(buffer, arg.units);
  buffer.Advance(1);
}


bool Group50Var4::ReadTarget(ReadBufferView& buff, TimeAndInterval& output)
{
  Group50Var4 value;
  if(Read(buff, value))
  {
    output = TimeAndIntervalFactory::From(value.time, value.interval, value.units);
    return true;
  }
  else
  {
    return false;
  }
}

void Group50Var4::WriteTarget(const TimeAndInterval& value, openpal::WriteBufferView& buff)
{
  Group50Var4::Write(ConvertGroup50Var4::Apply(value), buff);
}



}
