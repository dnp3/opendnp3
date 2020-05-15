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

#include "Group50.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group50Var1 -------

Group50Var1::Group50Var1() : time(0)
{}

bool Group50Var1::Read(rseq_t& buffer, Group50Var1& output)
{
  return LittleEndian::read(buffer, output.time);
}

bool Group50Var1::Write(const Group50Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.time);
}

// ------- Group50Var3 -------

Group50Var3::Group50Var3() : time(0)
{}

bool Group50Var3::Read(rseq_t& buffer, Group50Var3& output)
{
  return LittleEndian::read(buffer, output.time);
}

bool Group50Var3::Write(const Group50Var3& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.time);
}

// ------- Group50Var4 -------

Group50Var4::Group50Var4() : time(0), interval(0), units(0)
{}

bool Group50Var4::Read(rseq_t& buffer, Group50Var4& output)
{
  return LittleEndian::read(buffer, output.time, output.interval, output.units);
}

bool Group50Var4::Write(const Group50Var4& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.time, arg.interval, arg.units);
}

bool Group50Var4::ReadTarget(rseq_t& buff, TimeAndInterval& output)
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

bool Group50Var4::WriteTarget(const TimeAndInterval& value, ser4cpp::wseq_t& buff)
{
  return Group50Var4::Write(ConvertGroup50Var4::Apply(value), buff);
}


}
