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

#include "Group41.h"

#include "../PackingUnpacking.h"

namespace opendnp3 {

Group41Var1 Group41Var1::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group41Var1 obj;
  obj.value = Int32LE::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(*buffer);
  buffer.Advance(1);
  return obj;
}

Group41Var2 Group41Var2::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group41Var2 obj;
  obj.value = Int16LE::Read(buffer);
  buffer.Advance(2);
  obj.status = CommandStatusFromType(*buffer);
  buffer.Advance(1);
  return obj;
}

Group41Var3 Group41Var3::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group41Var3 obj;
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(*buffer);
  buffer.Advance(1);
  return obj;
}

Group41Var4 Group41Var4::Read(openpal::ReadOnlyBuffer& buffer)
{
  Group41Var4 obj;
  obj.value = SingleFloat::Read(buffer);
  buffer.Advance(4);
  obj.status = CommandStatusFromType(*buffer);
  buffer.Advance(1);
  return obj;
}


}
