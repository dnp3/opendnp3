//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group120.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group120Var3 Group120Var3::Read(ReadBufferView& buffer)
{
  Group120Var3 obj;
  obj.challengeSeqNum = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.userNum = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group120Var3::Write(const Group120Var3& arg, openpal::WriteBufferView& buffer)
{
  UInt32::Write(buffer, arg.challengeSeqNum);
  buffer.Advance(4);
  UInt16::Write(buffer, arg.userNum);
  buffer.Advance(2);
}



}
