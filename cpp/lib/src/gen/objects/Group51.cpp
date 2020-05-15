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

#include "Group51.h"

#include "app/parsing/DNPTimeParsing.h"
#include <ser4cpp/serialization/LittleEndian.h>

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group51Var1 -------

Group51Var1::Group51Var1() : time(0)
{}

bool Group51Var1::Read(rseq_t& buffer, Group51Var1& output)
{
  return LittleEndian::read(buffer, output.time);
}

bool Group51Var1::Write(const Group51Var1& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.time);
}

// ------- Group51Var2 -------

Group51Var2::Group51Var2() : time(0)
{}

bool Group51Var2::Read(rseq_t& buffer, Group51Var2& output)
{
  return LittleEndian::read(buffer, output.time);
}

bool Group51Var2::Write(const Group51Var2& arg, ser4cpp::wseq_t& buffer)
{
  return LittleEndian::write(buffer, arg.time);
}


}
