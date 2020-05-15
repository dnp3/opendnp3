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

#include "Group20.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group20Var1 -------

Group20Var1::Group20Var1() : flags(0), value(0)
{}

bool Group20Var1::Read(rseq_t& buffer, Group20Var1& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group20Var1::Write(const Group20Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group20Var1::ReadTarget(rseq_t& buff, Counter& output)
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

bool Group20Var1::WriteTarget(const Counter& value, ser4cpp::wseq_t& buff)
{
  return Group20Var1::Write(ConvertGroup20Var1::Apply(value), buff);
}

// ------- Group20Var2 -------

Group20Var2::Group20Var2() : flags(0), value(0)
{}

bool Group20Var2::Read(rseq_t& buffer, Group20Var2& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group20Var2::Write(const Group20Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group20Var2::ReadTarget(rseq_t& buff, Counter& output)
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

bool Group20Var2::WriteTarget(const Counter& value, ser4cpp::wseq_t& buff)
{
  return Group20Var2::Write(ConvertGroup20Var2::Apply(value), buff);
}

// ------- Group20Var5 -------

Group20Var5::Group20Var5() : value(0)
{}

bool Group20Var5::Read(rseq_t& buffer, Group20Var5& output)
{
  return LittleEndian::read(buffer, output.value);
}

bool Group20Var5::Write(const Group20Var5& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value);
}

bool Group20Var5::ReadTarget(rseq_t& buff, Counter& output)
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

bool Group20Var5::WriteTarget(const Counter& value, ser4cpp::wseq_t& buff)
{
  return Group20Var5::Write(ConvertGroup20Var5::Apply(value), buff);
}

// ------- Group20Var6 -------

Group20Var6::Group20Var6() : value(0)
{}

bool Group20Var6::Read(rseq_t& buffer, Group20Var6& output)
{
  return LittleEndian::read(buffer, output.value);
}

bool Group20Var6::Write(const Group20Var6& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value);
}

bool Group20Var6::ReadTarget(rseq_t& buff, Counter& output)
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

bool Group20Var6::WriteTarget(const Counter& value, ser4cpp::wseq_t& buff)
{
  return Group20Var6::Write(ConvertGroup20Var6::Apply(value), buff);
}


}
