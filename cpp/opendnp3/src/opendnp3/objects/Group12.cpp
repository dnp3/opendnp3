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

#include "Group12.h"

#include "../PackingUnpacking.h"

namespace opendnp3 {

Group12Var1 Group12Var1::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group12Var1 obj;
  obj.code = ControlCodeFromType(*buffer);
  buffer.Advance(1);
  obj.count = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.onTime = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.offTime = UInt32LE::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(*buffer);
  buffer.Advance(1);
  return obj;
}


}
