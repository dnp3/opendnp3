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

#include "Group41.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group41Var1 -------

Group41Var1::Group41Var1() : value(0), status(0)
{}

bool Group41Var1::Read(rseq_t& buffer, Group41Var1& output)
{
  return LittleEndian::read(buffer, output.value, output.status);
}

bool Group41Var1::Write(const Group41Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value, arg.status);
}

bool Group41Var1::ReadTarget(rseq_t& buff, AnalogOutputInt32& output)
{
  Group41Var1 value;
  if(Read(buff, value))
  {
    output = AnalogOutputInt32Factory::From(value.value, value.status);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group41Var1::WriteTarget(const AnalogOutputInt32& value, ser4cpp::wseq_t& buff)
{
  return Group41Var1::Write(ConvertGroup41Var1::Apply(value), buff);
}

// ------- Group41Var2 -------

Group41Var2::Group41Var2() : value(0), status(0)
{}

bool Group41Var2::Read(rseq_t& buffer, Group41Var2& output)
{
  return LittleEndian::read(buffer, output.value, output.status);
}

bool Group41Var2::Write(const Group41Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value, arg.status);
}

bool Group41Var2::ReadTarget(rseq_t& buff, AnalogOutputInt16& output)
{
  Group41Var2 value;
  if(Read(buff, value))
  {
    output = AnalogOutputInt16Factory::From(value.value, value.status);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group41Var2::WriteTarget(const AnalogOutputInt16& value, ser4cpp::wseq_t& buff)
{
  return Group41Var2::Write(ConvertGroup41Var2::Apply(value), buff);
}

// ------- Group41Var3 -------

Group41Var3::Group41Var3() : value(0.0), status(0)
{}

bool Group41Var3::Read(rseq_t& buffer, Group41Var3& output)
{
  return LittleEndian::read(buffer, output.value, output.status);
}

bool Group41Var3::Write(const Group41Var3& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value, arg.status);
}

bool Group41Var3::ReadTarget(rseq_t& buff, AnalogOutputFloat32& output)
{
  Group41Var3 value;
  if(Read(buff, value))
  {
    output = AnalogOutputFloat32Factory::From(value.value, value.status);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group41Var3::WriteTarget(const AnalogOutputFloat32& value, ser4cpp::wseq_t& buff)
{
  return Group41Var3::Write(ConvertGroup41Var3::Apply(value), buff);
}

// ------- Group41Var4 -------

Group41Var4::Group41Var4() : value(0.0), status(0)
{}

bool Group41Var4::Read(rseq_t& buffer, Group41Var4& output)
{
  return LittleEndian::read(buffer, output.value, output.status);
}

bool Group41Var4::Write(const Group41Var4& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value, arg.status);
}

bool Group41Var4::ReadTarget(rseq_t& buff, AnalogOutputDouble64& output)
{
  Group41Var4 value;
  if(Read(buff, value))
  {
    output = AnalogOutputDouble64Factory::From(value.value, value.status);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group41Var4::WriteTarget(const AnalogOutputDouble64& value, ser4cpp::wseq_t& buff)
{
  return Group41Var4::Write(ConvertGroup41Var4::Apply(value), buff);
}


}
