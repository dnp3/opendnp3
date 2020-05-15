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
// Copyright 2013-2020 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3_GROUP70_H
#define OPENDNP3_GROUP70_H

#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3 {

// File-control - File identifier
struct Group70Var1
{
  static GroupVariationID ID() { return GroupVariationID(70,1); }
};

// File-control - Authentication
struct Group70Var2
{
  static GroupVariationID ID() { return GroupVariationID(70,2); }
};

// File-control - File command
struct Group70Var3
{
  static GroupVariationID ID() { return GroupVariationID(70,3); }
};

// File-control - File command status
struct Group70Var4
{
  static GroupVariationID ID() { return GroupVariationID(70,4); }
};

// File-control - File transport
struct Group70Var5
{
  static GroupVariationID ID() { return GroupVariationID(70,5); }
};

// File-control - File transport status
struct Group70Var6
{
  static GroupVariationID ID() { return GroupVariationID(70,6); }
};

// File-control - File descriptor
struct Group70Var7
{
  static GroupVariationID ID() { return GroupVariationID(70,7); }
};

// File-control - File specification string
struct Group70Var8
{
  static GroupVariationID ID() { return GroupVariationID(70,8); }
};


}

#endif
