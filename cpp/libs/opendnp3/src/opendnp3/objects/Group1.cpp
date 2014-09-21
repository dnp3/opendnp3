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

#include "Group1.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group1Var2 Group1Var2::Read(ReadOnlyBuffer& buffer)
{
  Group1Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

void Group1Var2::Write(const Group1Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}


Binary Group1Var2::ReadTarget(ReadOnlyBuffer& buff)
{
  auto gv = Group1Var2::Read(buff);
  return BinaryFactory::From(gv.flags);
}

void Group1Var2::WriteTarget(const Binary& value, openpal::WriteBuffer& buff)
{
  Group1Var2::Write(ConvertGroup1Var2::Apply(value), buff);
}



}
