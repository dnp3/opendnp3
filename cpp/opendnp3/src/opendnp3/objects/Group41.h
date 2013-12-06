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

#include <openpal/BufferWrapper.h>
#include <opendnp3/gen/CommandStatus.h>
#include <opendnp3/gen/CommandStatus.h>
#include <opendnp3/gen/CommandStatus.h>
#include <opendnp3/gen/CommandStatus.h>

namespace opendnp3 {

struct Group41Var1
{
  static const size_t SIZE = 5;
  static Group41Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var1&, openpal::WriteBuffer&);

  int32_t value;
  CommandStatus status;
};

struct Group41Var2
{
  static const size_t SIZE = 3;
  static Group41Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var2&, openpal::WriteBuffer&);

  int16_t value;
  CommandStatus status;
};

struct Group41Var3
{
  static const size_t SIZE = 5;
  static Group41Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var3&, openpal::WriteBuffer&);

  float value;
  CommandStatus status;
};

struct Group41Var4
{
  static const size_t SIZE = 9;
  static Group41Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var4&, openpal::WriteBuffer&);

  double value;
  CommandStatus status;
};


}
