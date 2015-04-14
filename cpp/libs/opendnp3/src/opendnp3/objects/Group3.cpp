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

#include "Group3.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

bool Group3Var2::Read(ReadBufferView& buffer, Group3Var2& output)
{
  return Parse::Many(buffer, output.flags);
}

bool Group3Var2::Write(const Group3Var2& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.flags);
}

bool Group3Var2::ReadTarget(ReadBufferView& buff, DoubleBitBinary& output)
{
  Group3Var2 value;
  if(Read(buff, value))
  {
    output = DoubleBitBinaryFactory::From(value.flags);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group3Var2::WriteTarget(const DoubleBitBinary& value, openpal::WriteBufferView& buff)
{
  return Group3Var2::Write(ConvertGroup3Var2::Apply(value), buff);
}


}
