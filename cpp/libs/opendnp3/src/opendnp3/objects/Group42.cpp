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

#include "Group42.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group42Var1::Read(ReadBufferView& buffer, Group42Var1& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group42Var1::Write(const Group42Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
}


bool Group42Var1::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var1 value;
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

void Group42Var1::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var1::Write(ConvertGroup42Var1::Apply(value), buff);
}


bool Group42Var2::Read(ReadBufferView& buffer, Group42Var2& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group42Var2::Write(const Group42Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
}


bool Group42Var2::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var2 value;
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

void Group42Var2::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var2::Write(ConvertGroup42Var2::Apply(value), buff);
}


bool Group42Var3::Read(ReadBufferView& buffer, Group42Var3& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group42Var3::Write(const Group42Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group42Var3::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var3 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group42Var3::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var3::Write(ConvertGroup42Var3::Apply(value), buff);
}


bool Group42Var4::Read(ReadBufferView& buffer, Group42Var4& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group42Var4::Write(const Group42Var4& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  Int16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group42Var4::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var4 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group42Var4::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var4::Write(ConvertGroup42Var4::Apply(value), buff);
}


bool Group42Var5::Read(ReadBufferView& buffer, Group42Var5& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group42Var5::Write(const Group42Var5& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
}


bool Group42Var5::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var5 value;
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

void Group42Var5::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var5::Write(ConvertGroup42Var5::Apply(value), buff);
}


bool Group42Var6::Read(ReadBufferView& buffer, Group42Var6& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group42Var6::Write(const Group42Var6& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
}


bool Group42Var6::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var6 value;
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

void Group42Var6::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var6::Write(ConvertGroup42Var6::Apply(value), buff);
}


bool Group42Var7::Read(ReadBufferView& buffer, Group42Var7& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group42Var7::Write(const Group42Var7& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  SingleFloat::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group42Var7::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var7 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group42Var7::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var7::Write(ConvertGroup42Var7::Apply(value), buff);
}


bool Group42Var8::Read(ReadBufferView& buffer, Group42Var8& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group42Var8::Write(const Group42Var8& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  DoubleFloat::Write(buffer, arg.value);
  buffer.Advance(8);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group42Var8::ReadTarget(ReadBufferView& buff, AnalogOutputStatus& output)
{
  Group42Var8 value;
  if(Read(buff, value))
  {
    output = AnalogOutputStatusFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group42Var8::WriteTarget(const AnalogOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group42Var8::Write(ConvertGroup42Var8::Apply(value), buff);
}



}
