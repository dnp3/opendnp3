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

struct Group10Var2
{
  static const size_t SIZE = 1;

  typedef ControlStatus Target;
  static ControlStatus Convert(openpal::ReadOnlyBuffer&);

  static Group10Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group10Var2&, openpal::WriteBuffer&);

  uint8_t flags;
};


}
