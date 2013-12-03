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

#include "Group23.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group23Var1 Group23Var1::Read(ReadOnlyBuffer& buffer)
{
  Group23Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group23Var2 Group23Var2::Read(ReadOnlyBuffer& buffer)
{
  Group23Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group23Var3 Group23Var3::Read(ReadOnlyBuffer& buffer)
{
  Group23Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group23Var4 Group23Var4::Read(ReadOnlyBuffer& buffer)
{
  Group23Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group23Var5 Group23Var5::Read(ReadOnlyBuffer& buffer)
{
  Group23Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group23Var6 Group23Var6::Read(ReadOnlyBuffer& buffer)
{
  Group23Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group23Var7 Group23Var7::Read(ReadOnlyBuffer& buffer)
{
  Group23Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group23Var8 Group23Var8::Read(ReadOnlyBuffer& buffer)
{
  Group23Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}


}
