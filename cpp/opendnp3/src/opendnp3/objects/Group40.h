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

#ifndef __OPENDNP3_GENERATED_GROUP40_H_
#define __OPENDNP3_GENERATED_GROUP40_H_

#include <openpal/BufferWrapper.h>
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group40Var1
{
  static const size_t SIZE = 5;

  typedef SetpointStatus Target;
  static SetpointStatus Convert(openpal::ReadOnlyBuffer&);

  static Group40Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  int32_t value;
};

struct Group40Var2
{
  static const size_t SIZE = 3;

  typedef SetpointStatus Target;
  static SetpointStatus Convert(openpal::ReadOnlyBuffer&);

  static Group40Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  int16_t value;
};

struct Group40Var3
{
  static const size_t SIZE = 5;

  typedef SetpointStatus Target;
  static SetpointStatus Convert(openpal::ReadOnlyBuffer&);

  static Group40Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  float value;
};

struct Group40Var4
{
  static const size_t SIZE = 9;

  typedef SetpointStatus Target;
  static SetpointStatus Convert(openpal::ReadOnlyBuffer&);

  static Group40Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group40Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  double value;
};


}

#endif
