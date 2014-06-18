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

#ifndef __OPENDNP3_GENERATED_STATICANALOGRESPONSE_H_
#define __OPENDNP3_GENERATED_STATICANALOGRESPONSE_H_

#include <cstdint>

namespace opendnp3 {

enum class StaticAnalogResponse : int
{
  Group30Var1 = 0,
  Group30Var2 = 1,
  Group30Var3 = 2,
  Group30Var4 = 3,
  Group30Var5 = 4,
  Group30Var6 = 5
};

int StaticAnalogResponseToType(StaticAnalogResponse arg);
StaticAnalogResponse StaticAnalogResponseFromType(int arg);

}

#endif
