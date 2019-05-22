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

#include "Group51.h"

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

// ------- Group51Var1 -------

Group51Var1::Group51Var1() : time(0)
{}

bool Group51Var1::Read(RSlice& buffer, Group51Var1& output)
{
  return Parse::Many(buffer, output.time);
}

bool Group51Var1::Write(const Group51Var1& arg, openpal::WSlice& buffer)
{
  return Format::Many(buffer, arg.time);
}

// ------- Group51Var2 -------

Group51Var2::Group51Var2() : time(0)
{}

bool Group51Var2::Read(RSlice& buffer, Group51Var2& output)
{
  return Parse::Many(buffer, output.time);
}

bool Group51Var2::Write(const Group51Var2& arg, openpal::WSlice& buffer)
{
  return Format::Many(buffer, arg.time);
}


}
