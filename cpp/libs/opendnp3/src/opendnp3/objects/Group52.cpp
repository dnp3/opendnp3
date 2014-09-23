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
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group52.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group52Var1 Group52Var1::Read(ReadOnlyBuffer& buffer)
{
  Group52Var1 obj;
  obj.time = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group52Var1::Write(const Group52Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.time);
  buffer.Advance(2);
}


Group52Var2 Group52Var2::Read(ReadOnlyBuffer& buffer)
{
  Group52Var2 obj;
  obj.time = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group52Var2::Write(const Group52Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.time);
  buffer.Advance(2);
}



}
