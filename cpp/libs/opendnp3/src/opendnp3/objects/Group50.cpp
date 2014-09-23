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

#include "Group50.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group50Var1 Group50Var1::Read(ReadOnlyBuffer& buffer)
{
  Group50Var1 obj;
  obj.time = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group50Var1::Write(const Group50Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt48::Write(buffer, arg.time);
  buffer.Advance(6);
}



}
