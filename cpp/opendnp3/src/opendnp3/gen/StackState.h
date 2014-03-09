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

#ifndef __OPENDNP3_GENERATED_STACKSTATE_H_
#define __OPENDNP3_GENERATED_STACKSTATE_H_

#include <string>
#include <cstdint>

namespace opendnp3
{

/**
  Enumeration for possible communication states of a stack
*/
enum class StackState : int
{
    /// communications are online
    COMMS_UP = 0,
    /// communication difficulties
    COMMS_DOWN = 1,
    /// unknown state
    UNKNOWN = 2
};

std::string StackStateToString(StackState arg);

}

#endif
