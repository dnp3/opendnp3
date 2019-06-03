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

#include "Group32.h"

#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group32Var1 -------

Group32Var1::Group32Var1() : flags(0), value(0)
{}

bool Group32Var1::Read(rseq_t& buffer, Group32Var1& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= Int32::read_from(buffer, output.value);

  return result;
}

bool Group32Var1::Write(const Group32Var1& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= Int32::write_to(buffer, arg.value);

  return result;
}

bool Group32Var1::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var1 value;
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

bool Group32Var1::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var1::Write(ConvertGroup32Var1::Apply(value), buff);
}

// ------- Group32Var2 -------

Group32Var2::Group32Var2() : flags(0), value(0)
{}

bool Group32Var2::Read(rseq_t& buffer, Group32Var2& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= Int16::read_from(buffer, output.value);

  return result;
}

bool Group32Var2::Write(const Group32Var2& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= Int16::write_to(buffer, arg.value);

  return result;
}

bool Group32Var2::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var2 value;
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

bool Group32Var2::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var2::Write(ConvertGroup32Var2::Apply(value), buff);
}

// ------- Group32Var3 -------

Group32Var3::Group32Var3() : flags(0), value(0), time(0)
{}

bool Group32Var3::Read(rseq_t& buffer, Group32Var3& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= Int32::read_from(buffer, output.value);
  UInt48Type timeTemp;
  result &= UInt48::read_from(buffer, timeTemp);
  output.time = timeTemp.Get();

  return result;
}

bool Group32Var3::Write(const Group32Var3& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= Int32::write_to(buffer, arg.value);
  result &= UInt48::write_to(buffer, UInt48Type(arg.time));

  return result;
}

bool Group32Var3::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var3 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group32Var3::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var3::Write(ConvertGroup32Var3::Apply(value), buff);
}

// ------- Group32Var4 -------

Group32Var4::Group32Var4() : flags(0), value(0), time(0)
{}

bool Group32Var4::Read(rseq_t& buffer, Group32Var4& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= Int16::read_from(buffer, output.value);
  UInt48Type timeTemp;
  result &= UInt48::read_from(buffer, timeTemp);
  output.time = timeTemp.Get();

  return result;
}

bool Group32Var4::Write(const Group32Var4& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= Int16::write_to(buffer, arg.value);
  result &= UInt48::write_to(buffer, UInt48Type(arg.time));

  return result;
}

bool Group32Var4::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var4 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group32Var4::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var4::Write(ConvertGroup32Var4::Apply(value), buff);
}

// ------- Group32Var5 -------

Group32Var5::Group32Var5() : flags(0), value(0.0)
{}

bool Group32Var5::Read(rseq_t& buffer, Group32Var5& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= SingleFloat::read_from(buffer, output.value);

  return result;
}

bool Group32Var5::Write(const Group32Var5& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= SingleFloat::write_to(buffer, arg.value);

  return result;
}

bool Group32Var5::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var5 value;
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

bool Group32Var5::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var5::Write(ConvertGroup32Var5::Apply(value), buff);
}

// ------- Group32Var6 -------

Group32Var6::Group32Var6() : flags(0), value(0.0)
{}

bool Group32Var6::Read(rseq_t& buffer, Group32Var6& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= DoubleFloat::read_from(buffer, output.value);

  return result;
}

bool Group32Var6::Write(const Group32Var6& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= DoubleFloat::write_to(buffer, arg.value);

  return result;
}

bool Group32Var6::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var6 value;
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

bool Group32Var6::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var6::Write(ConvertGroup32Var6::Apply(value), buff);
}

// ------- Group32Var7 -------

Group32Var7::Group32Var7() : flags(0), value(0.0), time(0)
{}

bool Group32Var7::Read(rseq_t& buffer, Group32Var7& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= SingleFloat::read_from(buffer, output.value);
  UInt48Type timeTemp;
  result &= UInt48::read_from(buffer, timeTemp);
  output.time = timeTemp.Get();

  return result;
}

bool Group32Var7::Write(const Group32Var7& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= SingleFloat::write_to(buffer, arg.value);
  result &= UInt48::write_to(buffer, UInt48Type(arg.time));

  return result;
}

bool Group32Var7::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var7 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group32Var7::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var7::Write(ConvertGroup32Var7::Apply(value), buff);
}

// ------- Group32Var8 -------

Group32Var8::Group32Var8() : flags(0), value(0.0), time(0)
{}

bool Group32Var8::Read(rseq_t& buffer, Group32Var8& output)
{
  bool result = true;

  result &= UInt8::read_from(buffer, output.flags);
  result &= DoubleFloat::read_from(buffer, output.value);
  UInt48Type timeTemp;
  result &= UInt48::read_from(buffer, timeTemp);
  output.time = timeTemp.Get();

  return result;
}

bool Group32Var8::Write(const Group32Var8& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt8::write_to(buffer, arg.flags);
  result &= DoubleFloat::write_to(buffer, arg.value);
  result &= UInt48::write_to(buffer, UInt48Type(arg.time));

  return result;
}

bool Group32Var8::ReadTarget(rseq_t& buff, Analog& output)
{
  Group32Var8 value;
  if(Read(buff, value))
  {
    output = AnalogFactory::From(value.flags, value.value, value.time);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group32Var8::WriteTarget(const Analog& value, ser4cpp::wseq_t& buff)
{
  return Group32Var8::Write(ConvertGroup32Var8::Apply(value), buff);
}


}
