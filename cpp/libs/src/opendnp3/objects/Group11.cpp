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
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "Group11.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>
#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3 {

// ------- Group11Var1 -------

Group11Var1::Group11Var1() : flags(0)
{}

bool Group11Var1::Read(RSlice& buffer, Group11Var1& output)
{
  return Parse::Many(buffer, output.flags);
}

bool Group11Var1::Write(const Group11Var1& arg, openpal::WSlice& buffer)
{
  return Format::Many(buffer, arg.flags);
}

bool Group11Var1::ReadTarget(RSlice& buff, BinaryOutputStatus& output)
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

bool Group11Var1::WriteTarget(const BinaryOutputStatus& value, openpal::WSlice& buff)
{
  return Group11Var1::Write(ConvertGroup11Var1::Apply(value), buff);
}

// ------- Group11Var2 -------

Group11Var2::Group11Var2() : flags(0), time(0)
{}

bool Group11Var2::Read(RSlice& buffer, Group11Var2& output)
{
  return Parse::Many(buffer, output.flags, output.time);
}

bool Group11Var2::Write(const Group11Var2& arg, openpal::WSlice& buffer)
{
  return Format::Many(buffer, arg.flags, arg.time);
}

bool Group11Var2::ReadTarget(RSlice& buff, BinaryOutputStatus& output)
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

bool Group11Var2::WriteTarget(const BinaryOutputStatus& value, openpal::WSlice& buff)
{
  return Group11Var2::Write(ConvertGroup11Var2::Apply(value), buff);
}


}
