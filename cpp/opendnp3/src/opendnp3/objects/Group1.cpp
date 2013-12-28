//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group1.h"

#include "MeasurementFactory.h"
#include "../WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group1Var0::ID(1,0);

const GroupVariationID  Group1Var2::ID(1,2);

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

Binary Group1Var2::ReadAndConvert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return BinaryFactory::From(gv.flags);
}

void Group1Var2::ConvertAndWrite(const Binary& value, openpal::WriteBuffer& buff)
{
  Write(ConvertGroup1Var2::Apply(value), buff);
}


const GroupVariationID  Group1Var1::ID(1,1);


}
