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

#include "Group2.h"

#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group2Var1 Group2Var1::Read(ReadOnlyBuffer& buffer)
{
  Group2Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  return obj;
}

Group2Var2 Group2Var2::Read(ReadOnlyBuffer& buffer)
{
  Group2Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time48 = UInt48LE::Read(buffer);
  buffer.Advance(6);
  return obj;
}

Group2Var3 Group2Var3::Read(ReadOnlyBuffer& buffer)
{
  Group2Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.time16 = UInt16LE::Read(buffer);
  buffer.Advance(2);
  return obj;
}


}
