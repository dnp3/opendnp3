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

#include "Group30.h"

#include "../PackingUnpacking.h"

namespace opendnp3 {

Group30Var1 Group30Var1::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group30Var2 Group30Var2::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group30Var3 Group30Var3::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var3 obj;
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group30Var4 Group30Var4::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var4 obj;
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group30Var5 Group30Var5::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group30Var6 Group30Var6::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group30Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}


}
