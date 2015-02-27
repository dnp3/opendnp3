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

#include "Group20.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group20Var1::Read(ReadBufferView& buffer, Group20Var1& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

bool Group20Var1::Write(const Group20Var1& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags, arg.value);
}


bool Group20Var1::ReadTarget(ReadBufferView& buff, Counter& output)
{
  Group20Var1 value;
  if(Read(buff, value))
  {
    output = CounterFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group20Var1::WriteTarget(const Counter& value, openpal::WriteBufferView& buff)
{
  return Group20Var1::Write(ConvertGroup20Var1::Apply(value), buff);
}


bool Group20Var2::Read(ReadBufferView& buffer, Group20Var2& output)
{
  return Parse::Many(buffer, output.flags, output.value);
}

bool Group20Var2::Write(const Group20Var2& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags, arg.value);
}


bool Group20Var2::ReadTarget(ReadBufferView& buff, Counter& output)
{
  Group20Var2 value;
  if(Read(buff, value))
  {
    output = CounterFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group20Var2::WriteTarget(const Counter& value, openpal::WriteBufferView& buff)
{
  return Group20Var2::Write(ConvertGroup20Var2::Apply(value), buff);
}


bool Group20Var5::Read(ReadBufferView& buffer, Group20Var5& output)
{
  return Parse::Many(buffer, output.value);
}

bool Group20Var5::Write(const Group20Var5& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.value);
}


bool Group20Var5::ReadTarget(ReadBufferView& buff, Counter& output)
{
  Group20Var5 value;
  if(Read(buff, value))
  {
    output = CounterFactory::From(value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group20Var5::WriteTarget(const Counter& value, openpal::WriteBufferView& buff)
{
  return Group20Var5::Write(ConvertGroup20Var5::Apply(value), buff);
}


bool Group20Var6::Read(ReadBufferView& buffer, Group20Var6& output)
{
  return Parse::Many(buffer, output.value);
}

bool Group20Var6::Write(const Group20Var6& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.value);
}


bool Group20Var6::ReadTarget(ReadBufferView& buff, Counter& output)
{
  Group20Var6 value;
  if(Read(buff, value))
  {
    output = CounterFactory::From(value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group20Var6::WriteTarget(const Counter& value, openpal::WriteBufferView& buff)
{
  return Group20Var6::Write(ConvertGroup20Var6::Apply(value), buff);
}



}
