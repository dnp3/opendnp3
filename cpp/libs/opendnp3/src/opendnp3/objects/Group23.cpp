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

#include "Group23.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group23Var1::Read(ReadBufferView& buffer, Group23Var1& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group23Var1::Write(const Group23Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
}


bool Group23Var1::ReadTarget(ReadBufferView& buff, FrozenCounter& output)
{
  Group23Var1 value;
  if(Read(buff, value))
  {
    output = FrozenCounterFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group23Var1::WriteTarget(const FrozenCounter& value, openpal::WriteBufferView& buff)
{
  Group23Var1::Write(ConvertGroup23Var1::Apply(value), buff);
}


bool Group23Var2::Read(ReadBufferView& buffer, Group23Var2& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

void Group23Var2::Write(const Group23Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
}


bool Group23Var2::ReadTarget(ReadBufferView& buff, FrozenCounter& output)
{
  Group23Var2 value;
  if(Read(buff, value))
  {
    output = FrozenCounterFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

void Group23Var2::WriteTarget(const FrozenCounter& value, openpal::WriteBufferView& buff)
{
  Group23Var2::Write(ConvertGroup23Var2::Apply(value), buff);
}


bool Group23Var5::Read(ReadBufferView& buffer, Group23Var5& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group23Var5::Write(const Group23Var5& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.value);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group23Var5::ReadTarget(ReadBufferView& buff, FrozenCounter& output)
{
  Group23Var5 value;
  if(Read(buff, value))
  {
    output = FrozenCounterFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group23Var5::WriteTarget(const FrozenCounter& value, openpal::WriteBufferView& buff)
{
  Group23Var5::Write(ConvertGroup23Var5::Apply(value), buff);
}


bool Group23Var6::Read(ReadBufferView& buffer, Group23Var6& output)
{
  return Parse::Many(buffer, output.flags, output.value, output.time);
}

void Group23Var6::Write(const Group23Var6& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.value);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group23Var6::ReadTarget(ReadBufferView& buff, FrozenCounter& output)
{
  Group23Var6 value;
  if(Read(buff, value))
  {
    output = FrozenCounterFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group23Var6::WriteTarget(const FrozenCounter& value, openpal::WriteBufferView& buff)
{
  Group23Var6::Write(ConvertGroup23Var6::Apply(value), buff);
}



}
