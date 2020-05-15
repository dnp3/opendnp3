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
// Copyright 2013-2020 Automatak, LLC
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

#include "Group23.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group23Var1 -------

Group23Var1::Group23Var1() : flags(0), value(0)
{}

bool Group23Var1::Read(rseq_t& buffer, Group23Var1& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group23Var1::Write(const Group23Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group23Var1::ReadTarget(rseq_t& buff, FrozenCounter& output)
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

bool Group23Var1::WriteTarget(const FrozenCounter& value, ser4cpp::wseq_t& buff)
{
  return Group23Var1::Write(ConvertGroup23Var1::Apply(value), buff);
}

// ------- Group23Var2 -------

Group23Var2::Group23Var2() : flags(0), value(0)
{}

bool Group23Var2::Read(rseq_t& buffer, Group23Var2& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group23Var2::Write(const Group23Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group23Var2::ReadTarget(rseq_t& buff, FrozenCounter& output)
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

bool Group23Var2::WriteTarget(const FrozenCounter& value, ser4cpp::wseq_t& buff)
{
  return Group23Var2::Write(ConvertGroup23Var2::Apply(value), buff);
}

// ------- Group23Var5 -------

Group23Var5::Group23Var5() : flags(0), value(0), time(0)
{}

bool Group23Var5::Read(rseq_t& buffer, Group23Var5& output)
{
  return LittleEndian::read(buffer, output.flags, output.value, output.time);
}

bool Group23Var5::Write(const Group23Var5& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value, arg.time);
}

bool Group23Var5::ReadTarget(rseq_t& buff, FrozenCounter& output)
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

bool Group23Var5::WriteTarget(const FrozenCounter& value, ser4cpp::wseq_t& buff)
{
  return Group23Var5::Write(ConvertGroup23Var5::Apply(value), buff);
}

// ------- Group23Var6 -------

Group23Var6::Group23Var6() : flags(0), value(0), time(0)
{}

bool Group23Var6::Read(rseq_t& buffer, Group23Var6& output)
{
  return LittleEndian::read(buffer, output.flags, output.value, output.time);
}

bool Group23Var6::Write(const Group23Var6& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value, arg.time);
}

bool Group23Var6::ReadTarget(rseq_t& buff, FrozenCounter& output)
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

bool Group23Var6::WriteTarget(const FrozenCounter& value, ser4cpp::wseq_t& buff)
{
  return Group23Var6::Write(ConvertGroup23Var6::Apply(value), buff);
}


}
