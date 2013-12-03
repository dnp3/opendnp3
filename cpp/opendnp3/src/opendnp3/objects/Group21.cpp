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

#include "Group21.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group21Var1 Group21Var1::Read(ReadOnlyBuffer& buffer)
{
  Group21Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group21Var2 Group21Var2::Read(ReadOnlyBuffer& buffer)
{
  Group21Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group21Var3 Group21Var3::Read(ReadOnlyBuffer& buffer)
{
  Group21Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group21Var4 Group21Var4::Read(ReadOnlyBuffer& buffer)
{
  Group21Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group21Var5 Group21Var5::Read(ReadOnlyBuffer& buffer)
{
  Group21Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group21Var6 Group21Var6::Read(ReadOnlyBuffer& buffer)
{
  Group21Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group21Var7 Group21Var7::Read(ReadOnlyBuffer& buffer)
{
  Group21Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group21Var8 Group21Var8::Read(ReadOnlyBuffer& buffer)
{
  Group21Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group21Var9 Group21Var9::Read(ReadOnlyBuffer& buffer)
{
  Group21Var9 obj;
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group21Var10 Group21Var10::Read(ReadOnlyBuffer& buffer)
{
  Group21Var10 obj;
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group21Var11 Group21Var11::Read(ReadOnlyBuffer& buffer)
{
  Group21Var11 obj;
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group21Var12 Group21Var12::Read(ReadOnlyBuffer& buffer)
{
  Group21Var12 obj;
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}


}
