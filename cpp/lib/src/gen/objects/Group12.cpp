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

#include "Group12.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>
#include "app/MeasurementFactory.h"
#include "app/WriteConversions.h"

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group12Var1 -------

Group12Var1::Group12Var1() : code(0), count(0), onTime(0), offTime(0), status(0)
{}

bool Group12Var1::Read(rseq_t& buffer, Group12Var1& output)
{
  return LittleEndian::read(buffer, output.code, output.count, output.onTime, output.offTime, output.status);
}

bool Group12Var1::Write(const Group12Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.code, arg.count, arg.onTime, arg.offTime, arg.status);
}

bool Group12Var1::ReadTarget(rseq_t& buff, ControlRelayOutputBlock& output)
{
  Group12Var1 value;
  if(Read(buff, value))
  {
    output = ControlRelayOutputBlockFactory::From(value.code, value.count, value.onTime, value.offTime, value.status);
    return true;
  }
  else
  {
    return false;
  }
}

bool Group12Var1::WriteTarget(const ControlRelayOutputBlock& value, ser4cpp::wseq_t& buff)
{
  return Group12Var1::Write(ConvertGroup12Var1::Apply(value), buff);
}


}
