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

#include "Group43.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group43Var1 -------

Group43Var1::Group43Var1() : status(0), value(0)
{}

bool Group43Var1::Read(rseq_t& buffer, Group43Var1& output)
{
  return LittleEndian::read(buffer, output.status, output.value);
}

bool Group43Var1::Write(const Group43Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value);
}

bool Group43Var1::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var1 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var1::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var1::Write(ConvertGroup43Var1::Apply(value), buff);
}

// ------- Group43Var2 -------

Group43Var2::Group43Var2() : status(0), value(0)
{}

bool Group43Var2::Read(rseq_t& buffer, Group43Var2& output)
{
  return LittleEndian::read(buffer, output.status, output.value);
}

bool Group43Var2::Write(const Group43Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value);
}

bool Group43Var2::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var2 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var2::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var2::Write(ConvertGroup43Var2::Apply(value), buff);
}

// ------- Group43Var3 -------

Group43Var3::Group43Var3() : status(0), value(0), time(0)
{}

bool Group43Var3::Read(rseq_t& buffer, Group43Var3& output)
{
  return LittleEndian::read(buffer, output.status, output.value, output.time);
}

bool Group43Var3::Write(const Group43Var3& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value, arg.time);
}

bool Group43Var3::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var3 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var3::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var3::Write(ConvertGroup43Var3::Apply(value), buff);
}

// ------- Group43Var4 -------

Group43Var4::Group43Var4() : status(0), value(0), time(0)
{}

bool Group43Var4::Read(rseq_t& buffer, Group43Var4& output)
{
  return LittleEndian::read(buffer, output.status, output.value, output.time);
}

bool Group43Var4::Write(const Group43Var4& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value, arg.time);
}

bool Group43Var4::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var4 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var4::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var4::Write(ConvertGroup43Var4::Apply(value), buff);
}

// ------- Group43Var5 -------

Group43Var5::Group43Var5() : status(0), value(0.0)
{}

bool Group43Var5::Read(rseq_t& buffer, Group43Var5& output)
{
  return LittleEndian::read(buffer, output.status, output.value);
}

bool Group43Var5::Write(const Group43Var5& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value);
}

bool Group43Var5::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var5 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var5::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var5::Write(ConvertGroup43Var5::Apply(value), buff);
}

// ------- Group43Var6 -------

Group43Var6::Group43Var6() : status(0), value(0.0)
{}

bool Group43Var6::Read(rseq_t& buffer, Group43Var6& output)
{
  return LittleEndian::read(buffer, output.status, output.value);
}

bool Group43Var6::Write(const Group43Var6& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value);
}

bool Group43Var6::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var6 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var6::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var6::Write(ConvertGroup43Var6::Apply(value), buff);
}

// ------- Group43Var7 -------

Group43Var7::Group43Var7() : status(0), value(0.0), time(0)
{}

bool Group43Var7::Read(rseq_t& buffer, Group43Var7& output)
{
  return LittleEndian::read(buffer, output.status, output.value, output.time);
}

bool Group43Var7::Write(const Group43Var7& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value, arg.time);
}

bool Group43Var7::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var7 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var7::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var7::Write(ConvertGroup43Var7::Apply(value), buff);
}

// ------- Group43Var8 -------

Group43Var8::Group43Var8() : status(0), value(0.0), time(0)
{}

bool Group43Var8::Read(rseq_t& buffer, Group43Var8& output)
{
  return LittleEndian::read(buffer, output.status, output.value, output.time);
}

bool Group43Var8::Write(const Group43Var8& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.status, arg.value, arg.time);
}

bool Group43Var8::ReadTarget(rseq_t& buff, AnalogCommandEvent& output)
{
  Group43Var8 value;
  if(Read(buff, value))
  {
    output = AnalogCommandEventFactory::From(value.status, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group43Var8::WriteTarget(const AnalogCommandEvent& value, ser4cpp::wseq_t& buff)
{
  return Group43Var8::Write(ConvertGroup43Var8::Apply(value), buff);
}


}
