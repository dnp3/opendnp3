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

#include "Group13.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

bool Group13Var1::Read(ReadBufferView& buffer, Group13Var1& output)
{
  return Parse::Many(buffer, output.flags);
}

bool Group13Var1::Write(const Group13Var1& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags);
}


bool Group13Var1::ReadTarget(ReadBufferView& buff, BinaryCommandEvent& output)
{
  Group13Var1 value;
  if(Read(buff, value))
  {
    output = BinaryCommandEventFactory::From(value.flags);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group13Var1::WriteTarget(const BinaryCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group13Var1::Write(ConvertGroup13Var1::Apply(value), buff);
}


bool Group13Var2::Read(ReadBufferView& buffer, Group13Var2& output)
{
  return Parse::Many(buffer, output.flags, output.time);
}

bool Group13Var2::Write(const Group13Var2& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags, arg.time);
}


bool Group13Var2::ReadTarget(ReadBufferView& buff, BinaryCommandEvent& output)
{
  Group13Var2 value;
  if(Read(buff, value))
  {
    output = BinaryCommandEventFactory::From(value.flags, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group13Var2::WriteTarget(const BinaryCommandEvent& value, openpal::WriteBufferView& buff)
{
  return Group13Var2::Write(ConvertGroup13Var2::Apply(value), buff);
}



}
