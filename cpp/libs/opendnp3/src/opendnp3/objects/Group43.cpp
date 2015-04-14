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

#include "Group43.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

bool Group43Var1::Read(ReadBufferView& buffer, Group43Var1& output)
{
  return Parse::Many(buffer, output.status, output.value);
}

bool Group43Var1::Write(const Group43Var1& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value);
}


bool Group43Var1::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var1 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var1::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var1::Write(ConvertGroup43Var1::Apply(value), buff);
}


bool Group43Var2::Read(ReadBufferView& buffer, Group43Var2& output)
{
  return Parse::Many(buffer, output.status, output.value);
}

bool Group43Var2::Write(const Group43Var2& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value);
}


bool Group43Var2::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var2 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var2::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var2::Write(ConvertGroup43Var2::Apply(value), buff);
}


bool Group43Var3::Read(ReadBufferView& buffer, Group43Var3& output)
{
  return Parse::Many(buffer, output.status, output.value, output.time);
}

bool Group43Var3::Write(const Group43Var3& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value, arg.time);
}


bool Group43Var3::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var3 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var3::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var3::Write(ConvertGroup43Var3::Apply(value), buff);
}


bool Group43Var4::Read(ReadBufferView& buffer, Group43Var4& output)
{
  return Parse::Many(buffer, output.status, output.value, output.time);
}

bool Group43Var4::Write(const Group43Var4& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value, arg.time);
}


bool Group43Var4::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var4 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var4::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var4::Write(ConvertGroup43Var4::Apply(value), buff);
}


bool Group43Var5::Read(ReadBufferView& buffer, Group43Var5& output)
{
  return Parse::Many(buffer, output.status, output.value);
}

bool Group43Var5::Write(const Group43Var5& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value);
}


bool Group43Var5::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var5 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var5::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var5::Write(ConvertGroup43Var5::Apply(value), buff);
}


bool Group43Var6::Read(ReadBufferView& buffer, Group43Var6& output)
{
  return Parse::Many(buffer, output.status, output.value);
}

bool Group43Var6::Write(const Group43Var6& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value);
}


bool Group43Var6::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var6 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var6::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var6::Write(ConvertGroup43Var6::Apply(value), buff);
}


bool Group43Var7::Read(ReadBufferView& buffer, Group43Var7& output)
{
  return Parse::Many(buffer, output.status, output.value, output.time);
}

bool Group43Var7::Write(const Group43Var7& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value, arg.time);
}


bool Group43Var7::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var7 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var7::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var7::Write(ConvertGroup43Var7::Apply(value), buff);
}


bool Group43Var8::Read(ReadBufferView& buffer, Group43Var8& output)
{
  return Parse::Many(buffer, output.status, output.value, output.time);
}

bool Group43Var8::Write(const Group43Var8& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.status, arg.value, arg.time);
}


bool Group43Var8::ReadTarget(ReadBufferView& buff, AnalogCommandEvent& output)
{
  Group43Var8 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var8::WriteTarget(const AnalogCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group43Var8::Write(ConvertGroup43Var8::Apply(value), buff);
}



}
