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

#include "Group2.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group2Var1 -------

Group2Var1::Group2Var1() : flags(0)
{}

bool Group2Var1::Read(rseq_t& buffer, Group2Var1& output)
{
  return LittleEndian::read(buffer, output.flags);
}

bool Group2Var1::Write(const Group2Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags);
}

bool Group2Var1::ReadTarget(rseq_t& buff, Binary& output)
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

bool Group2Var1::WriteTarget(const Binary& value, ser4cpp::wseq_t& buff)
{
  return Group2Var1::Write(ConvertGroup2Var1::Apply(value), buff);
}

// ------- Group2Var2 -------

Group2Var2::Group2Var2() : flags(0), time(0)
{}

bool Group2Var2::Read(rseq_t& buffer, Group2Var2& output)
{
  return LittleEndian::read(buffer, output.flags, output.time);
}

bool Group2Var2::Write(const Group2Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.time);
}

bool Group2Var2::ReadTarget(rseq_t& buff, Binary& output)
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

bool Group2Var2::WriteTarget(const Binary& value, ser4cpp::wseq_t& buff)
{
  return Group2Var2::Write(ConvertGroup2Var2::Apply(value), buff);
}

// ------- Group2Var3 -------

Group2Var3::Group2Var3() : flags(0), time(0)
{}

bool Group2Var3::Read(rseq_t& buffer, Group2Var3& output)
{
  return LittleEndian::read(buffer, output.flags, output.time);
}

bool Group2Var3::Write(const Group2Var3& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.time);
}

bool Group2Var3::ReadTarget(rseq_t& buff, Binary& output)
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

bool Group2Var3::WriteTarget(const Binary& value, ser4cpp::wseq_t& buff)
{
  return Group2Var3::Write(ConvertGroup2Var3::Apply(value), buff);
}


}
