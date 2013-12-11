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
#include <opendnp3/DataTypes.h>

namespace opendnp3 {

struct Group20Var1
{
  static const size_t SIZE = 5;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var1&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group20Var2
{
  static const size_t SIZE = 3;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var2&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group20Var3
{
  static const size_t SIZE = 5;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var3&, openpal::WriteBuffer&);

  uint8_t flags;
  uint32_t count;
};

struct Group20Var4
{
  static const size_t SIZE = 3;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var4&, openpal::WriteBuffer&);

  uint8_t flags;
  uint16_t count;
};

struct Group20Var5
{
  static const size_t SIZE = 4;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var5 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var5&, openpal::WriteBuffer&);

  uint32_t count;
};

struct Group20Var6
{
  static const size_t SIZE = 2;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var6 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var6&, openpal::WriteBuffer&);

  uint16_t count;
};

struct Group20Var7
{
  static const size_t SIZE = 4;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var7 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var7&, openpal::WriteBuffer&);

  uint32_t count;
};

struct Group20Var8
{
  static const size_t SIZE = 2;

  typedef Counter Target;
  static Counter Convert(openpal::ReadOnlyBuffer&);

  static Group20Var8 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group20Var8&, openpal::WriteBuffer&);

  uint16_t count;
};


}
