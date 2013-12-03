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

#include "Group42.h"

#include "../PackingUnpacking.h"

namespace opendnp3 {

Group42Var1 Group42Var1::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group42Var2 Group42Var2::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group42Var3 Group42Var3::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group42Var4 Group42Var4::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group42Var5 Group42Var5::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group42Var6 Group42Var6::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group42Var7 Group42Var7::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group42Var8 Group42Var8::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group42Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}


}
