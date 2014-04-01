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

#ifndef __OPENDNP3_GENERATED_EVENTANALOGRESPONSE_H_
#define __OPENDNP3_GENERATED_EVENTANALOGRESPONSE_H_

#include <string>
#include <cstdint>

namespace opendnp3
{

enum class EventAnalogResponse : int
{
    Group32Var1 = 0,
    Group32Var2 = 1,
    Group32Var3 = 2,
    Group32Var4 = 3,
    Group32Var5 = 4,
    Group32Var6 = 5,
    Group32Var7 = 6,
    Group32Var8 = 7
};

int EventAnalogResponseToType(EventAnalogResponse arg);
EventAnalogResponse EventAnalogResponseFromType(int arg);

}

#endif
