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

#include "Group32.h"

#include "../PackingUnpacking.h"

namespace opendnp3 {

Group32Var1 Group32Var1::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group32Var2 Group32Var2::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group32Var3 Group32Var3::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group32Var4 Group32Var4::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group32Var5 Group32Var5::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group32Var6 Group32Var6::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group32Var7 Group32Var7::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group32Var8 Group32Var8::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group32Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}


}
