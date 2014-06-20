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

#include "Group10.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

const GroupVariationID  Group10Var0::ID(10,0);

const GroupVariationID  Group10Var1::ID(10,1);

const GroupVariationID  Group10Var2::ID(10,2);

Group10Var2 Group10Var2::Read(ReadOnlyBuffer& buffer)
{
  Group10Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

void Group10Var2::Write(const Group10Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
}

Group10Var2Serializer Group10Var2Serializer::mInstance;

BinaryOutputStatus Group10Var2Serializer::Read(ReadOnlyBuffer& buff) const
{
  auto gv = Group10Var2::Read(buff);
  return BinaryOutputStatusFactory::From(gv.flags);
}

void Group10Var2Serializer::Write(const BinaryOutputStatus& value, openpal::WriteBuffer& buff) const
{
  Group10Var2::Write(ConvertGroup10Var2::Apply(value), buff);
}



}
