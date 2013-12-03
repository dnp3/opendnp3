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

#include "Group20.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group20Var1 Group20Var1::Read(ReadOnlyBuffer& buffer)
{
  Group20Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group20Var2 Group20Var2::Read(ReadOnlyBuffer& buffer)
{
  Group20Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group20Var3 Group20Var3::Read(ReadOnlyBuffer& buffer)
{
  Group20Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group20Var4 Group20Var4::Read(ReadOnlyBuffer& buffer)
{
  Group20Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group20Var5 Group20Var5::Read(ReadOnlyBuffer& buffer)
{
  Group20Var5 obj;
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group20Var6 Group20Var6::Read(ReadOnlyBuffer& buffer)
{
  Group20Var6 obj;
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group20Var7 Group20Var7::Read(ReadOnlyBuffer& buffer)
{
  Group20Var7 obj;
  obj.count = UInt32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group20Var8 Group20Var8::Read(ReadOnlyBuffer& buffer)
{
  Group20Var8 obj;
  obj.count = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}


}
