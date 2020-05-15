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

#include "Group30.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group30Var1 -------

Group30Var1::Group30Var1() : flags(0), value(0)
{}

bool Group30Var1::Read(rseq_t& buffer, Group30Var1& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group30Var1::Write(const Group30Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group30Var1::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var1 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var1::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var1::Write(ConvertGroup30Var1::Apply(value), buff);
}

// ------- Group30Var2 -------

Group30Var2::Group30Var2() : flags(0), value(0)
{}

bool Group30Var2::Read(rseq_t& buffer, Group30Var2& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group30Var2::Write(const Group30Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group30Var2::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var2 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var2::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var2::Write(ConvertGroup30Var2::Apply(value), buff);
}

// ------- Group30Var3 -------

Group30Var3::Group30Var3() : value(0)
{}

bool Group30Var3::Read(rseq_t& buffer, Group30Var3& output)
{
  return LittleEndian::read(buffer, output.value);
}

bool Group30Var3::Write(const Group30Var3& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value);
}

bool Group30Var3::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var3 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var3::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var3::Write(ConvertGroup30Var3::Apply(value), buff);
}

// ------- Group30Var4 -------

Group30Var4::Group30Var4() : value(0)
{}

bool Group30Var4::Read(rseq_t& buffer, Group30Var4& output)
{
  return LittleEndian::read(buffer, output.value);
}

bool Group30Var4::Write(const Group30Var4& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.value);
}

bool Group30Var4::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var4 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var4::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var4::Write(ConvertGroup30Var4::Apply(value), buff);
}

// ------- Group30Var5 -------

Group30Var5::Group30Var5() : flags(0), value(0.0)
{}

bool Group30Var5::Read(rseq_t& buffer, Group30Var5& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group30Var5::Write(const Group30Var5& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group30Var5::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var5 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var5::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var5::Write(ConvertGroup30Var5::Apply(value), buff);
}

// ------- Group30Var6 -------

Group30Var6::Group30Var6() : flags(0), value(0.0)
{}

bool Group30Var6::Read(rseq_t& buffer, Group30Var6& output)
{
  return LittleEndian::read(buffer, output.flags, output.value);
}

bool Group30Var6::Write(const Group30Var6& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.flags, arg.value);
}

bool Group30Var6::ReadTarget(rseq_t& buff, Analog& output)
{
  Group30Var6 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group30Var6::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group30Var6::Write(ConvertGroup30Var6::Apply(value), buff);
}


}
