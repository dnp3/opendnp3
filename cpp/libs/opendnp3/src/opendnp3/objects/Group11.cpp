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
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

bool Group11Var1::Read(ReadBufferView& buffer, Group11Var1& output)
{
  return Parse::Many(buffer, output.flags);
}

void Group11Var1::Write(const Group11Var1& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}


bool Group11Var1::ReadTarget(ReadBufferView& buff, BinaryOutputStatus& output)
{
  Group11Var1 value;
  if(Read(buff, value))
  {
    output = BinaryOutputStatusFactory::From(value.flags);
    return true;
  }
  else
  {
    return false;
  }
}

void Group11Var1::WriteTarget(const BinaryOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group11Var1::Write(ConvertGroup11Var1::Apply(value), buff);
}


bool Group11Var2::Read(ReadBufferView& buffer, Group11Var2& output)
{
  return Parse::Many(buffer, output.flags, output.time);
}

void Group11Var2::Write(const Group11Var2& arg, openpal::WriteBufferView& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}


bool Group11Var2::ReadTarget(ReadBufferView& buff, BinaryOutputStatus& output)
{
  Group11Var2 value;
  if(Read(buff, value))
  {
    output = BinaryOutputStatusFactory::From(value.flags, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

void Group11Var2::WriteTarget(const BinaryOutputStatus& value, openpal::WriteBufferView& buff)
{
  Group11Var2::Write(ConvertGroup11Var2::Apply(value), buff);
}



}
