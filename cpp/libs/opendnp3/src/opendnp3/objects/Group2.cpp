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

#include "Group2.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group2Var1::Read(ReadBufferView& buffer, Group2Var1& output)
{
  return Parse::Many(buffer, output.flags);
}

void Group2Var1::Write(const Group2Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}


bool Group2Var1::ReadTarget(ReadBufferView& buff, Binary& output)
{
  Group2Var1 value;
  if(Read(buff, value))
  {
    output = BinaryFactory::From(value.flags);
    return true;
  }
  else
  {
    return false;
  }
}

void Group2Var1::WriteTarget(const Binary& value, openpal::WriteBufferView& buff)
{
  Group2Var1::Write(ConvertGroup2Var1::Apply(value), buff);
}


bool Group2Var2::Read(ReadBufferView& buffer, Group2Var2& output)
{
  return Parse::Many(buffer, output.flags, output.time);
}

void Group2Var2::Write(const Group2Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group2Var2::ReadTarget(ReadBufferView& buff, Binary& output)
{
  Group2Var2 value;
  if(Read(buff, value))
  {
    output = BinaryFactory::From(value.flags, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group2Var2::WriteTarget(const Binary& value, openpal::WriteBufferView& buff)
{
  Group2Var2::Write(ConvertGroup2Var2::Apply(value), buff);
}


bool Group2Var3::Read(ReadBufferView& buffer, Group2Var3& output)
{
  return Parse::Many(buffer, output.flags, output.time);
}

void Group2Var3::Write(const Group2Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.time);
  buffer.Advance(2);
}


bool Group2Var3::ReadTarget(ReadBufferView& buff, Binary& output)
{
  Group2Var3 value;
  if(Read(buff, value))
  {
    output = BinaryFactory::From(value.flags, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group2Var3::WriteTarget(const Binary& value, openpal::WriteBufferView& buff)
{
  Group2Var3::Write(ConvertGroup2Var3::Apply(value), buff);
}



}
