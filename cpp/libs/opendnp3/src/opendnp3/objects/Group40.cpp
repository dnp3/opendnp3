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

#include "Group40.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group40Var1::Read(ReadBufferView& buffer, Group40Var1& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group40Var1::Write(const Group40Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}


bool Group40Var1::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group40Var1 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group40Var1::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group40Var1::Write(ConvertGroup40Var1::Apply(value), buff);
}


bool Group40Var2::Read(ReadBufferView& buffer, Group40Var2& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group40Var2::Write(const Group40Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}


bool Group40Var2::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group40Var2 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group40Var2::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group40Var2::Write(ConvertGroup40Var2::Apply(value), buff);
}


bool Group40Var3::Read(ReadBufferView& buffer, Group40Var3& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group40Var3::Write(const Group40Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}


bool Group40Var3::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group40Var3 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group40Var3::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group40Var3::Write(ConvertGroup40Var3::Apply(value), buff);
}


bool Group40Var4::Read(ReadBufferView& buffer, Group40Var4& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group40Var4::Write(const Group40Var4& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}


bool Group40Var4::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group40Var4 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group40Var4::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group40Var4::Write(ConvertGroup40Var4::Apply(value), buff);
}



}
