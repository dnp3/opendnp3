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

#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

namespace opendnp3 {

// ------- Group120Var1 -------

uint32_t Group120Var1::Size() const
{
  return MIN_SIZE + challengeData.Size();
}

// ------- Group120Var2 -------

uint32_t Group120Var2::Size() const
{
  return MIN_SIZE + hmac.Size();
}

// ------- Group120Var3 -------

bool Group120Var3::Read(ReadBufferView& buffer, Group120Var3& output)
{
  return Parse::Many(buffer, output.challengeSeqNum, output.userNum);
}

bool Group120Var3::Write(const Group120Var3& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.challengeSeqNum, arg.userNum);
}

// ------- Group120Var4 -------

bool Group120Var4::Read(ReadBufferView& buffer, Group120Var4& output)
{
  return Parse::Many(buffer, output.userNum);
}

bool Group120Var4::Write(const Group120Var4& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.userNum);
}

// ------- Group120Var5 -------

uint32_t Group120Var5::Size() const
{
  return MIN_SIZE + challengeData.Size() + hmac.Size();
}

// ------- Group120Var6 -------

uint32_t Group120Var6::Size() const
{
  return MIN_SIZE + keyWrapData.Size();
}

// ------- Group120Var7 -------

uint32_t Group120Var7::Size() const
{
  return MIN_SIZE + errorText.Size();
}

// ------- Group120Var9 -------

uint32_t Group120Var9::Size() const
{
  return MIN_SIZE + hmac.Size();
}


}
