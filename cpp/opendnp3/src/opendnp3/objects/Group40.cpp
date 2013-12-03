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

#include "Group40.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group40Var1 Group40Var1::Read(ReadOnlyBuffer& buffer)
{
  Group40Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group40Var2 Group40Var2::Read(ReadOnlyBuffer& buffer)
{
  Group40Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = Int16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

Group40Var3 Group40Var3::Read(ReadOnlyBuffer& buffer)
{
  Group40Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}

Group40Var4 Group40Var4::Read(ReadOnlyBuffer& buffer)
{
  Group40Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  return obj;
}


}
