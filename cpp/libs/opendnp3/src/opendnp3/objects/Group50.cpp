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

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

bool Group50Var1::Read(ReadBufferView& buffer, Group50Var1& output)
{
  return Parse::Many(buffer, output.time);
}

bool Group50Var1::Write(const Group50Var1& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.time);
}


bool Group50Var4::Read(ReadBufferView& buffer, Group50Var4& output)
{
  return Parse::Many(buffer, output.time, output.interval, output.units);
}

bool Group50Var4::Write(const Group50Var4& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.time, arg.interval, arg.units);
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

bool Group50Var4::WriteTarget(const TimeAndInterval& value, openpal::WriteBufferView& buff)
{
  return Group50Var4::Write(ConvertGroup50Var4::Apply(value), buff);
}



}
