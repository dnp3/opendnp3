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

#include "opendnp3/gen/Attributes.h"

namespace opendnp3 {

bool HasAbsoluteTime(GroupVariation gv)
{
  switch(gv)
  {
    case(GroupVariation::Group2Var2):
        return true;
    case(GroupVariation::Group4Var2):
        return true;
    case(GroupVariation::Group11Var2):
        return true;
    case(GroupVariation::Group13Var2):
        return true;
    case(GroupVariation::Group21Var5):
        return true;
    case(GroupVariation::Group21Var6):
        return true;
    case(GroupVariation::Group22Var5):
        return true;
    case(GroupVariation::Group22Var6):
        return true;
    case(GroupVariation::Group23Var5):
        return true;
    case(GroupVariation::Group23Var6):
        return true;
    case(GroupVariation::Group32Var3):
        return true;
    case(GroupVariation::Group32Var4):
        return true;
    case(GroupVariation::Group32Var7):
        return true;
    case(GroupVariation::Group32Var8):
        return true;
    case(GroupVariation::Group42Var3):
        return true;
    case(GroupVariation::Group42Var4):
        return true;
    case(GroupVariation::Group42Var7):
        return true;
    case(GroupVariation::Group42Var8):
        return true;
    case(GroupVariation::Group43Var3):
        return true;
    case(GroupVariation::Group43Var4):
        return true;
    case(GroupVariation::Group43Var7):
        return true;
    case(GroupVariation::Group43Var8):
        return true;
    case(GroupVariation::Group50Var1):
        return true;
    case(GroupVariation::Group50Var3):
        return true;
    case(GroupVariation::Group50Var4):
        return true;
    case(GroupVariation::Group51Var1):
        return true;
    case(GroupVariation::Group51Var2):
        return true;
    default:
      return false;
  }
}
bool HasRelativeTime(GroupVariation gv)
{
  switch(gv)
  {
    case(GroupVariation::Group2Var3):
        return true;
    case(GroupVariation::Group4Var3):
        return true;
    case(GroupVariation::Group52Var1):
        return true;
    case(GroupVariation::Group52Var2):
        return true;
    default:
      return false;
  }
}
bool HasFlags(GroupVariation gv)
{
  switch(gv)
  {
    case(GroupVariation::Group1Var2):
        return true;
    case(GroupVariation::Group2Var1):
        return true;
    case(GroupVariation::Group2Var2):
        return true;
    case(GroupVariation::Group2Var3):
        return true;
    case(GroupVariation::Group3Var2):
        return true;
    case(GroupVariation::Group4Var1):
        return true;
    case(GroupVariation::Group4Var2):
        return true;
    case(GroupVariation::Group4Var3):
        return true;
    case(GroupVariation::Group10Var2):
        return true;
    case(GroupVariation::Group11Var1):
        return true;
    case(GroupVariation::Group11Var2):
        return true;
    case(GroupVariation::Group13Var1):
        return true;
    case(GroupVariation::Group13Var2):
        return true;
    case(GroupVariation::Group20Var1):
        return true;
    case(GroupVariation::Group20Var2):
        return true;
    case(GroupVariation::Group21Var1):
        return true;
    case(GroupVariation::Group21Var2):
        return true;
    case(GroupVariation::Group21Var5):
        return true;
    case(GroupVariation::Group21Var6):
        return true;
    case(GroupVariation::Group22Var1):
        return true;
    case(GroupVariation::Group22Var2):
        return true;
    case(GroupVariation::Group22Var5):
        return true;
    case(GroupVariation::Group22Var6):
        return true;
    case(GroupVariation::Group23Var1):
        return true;
    case(GroupVariation::Group23Var2):
        return true;
    case(GroupVariation::Group23Var5):
        return true;
    case(GroupVariation::Group23Var6):
        return true;
    case(GroupVariation::Group30Var1):
        return true;
    case(GroupVariation::Group30Var2):
        return true;
    case(GroupVariation::Group30Var5):
        return true;
    case(GroupVariation::Group30Var6):
        return true;
    case(GroupVariation::Group32Var1):
        return true;
    case(GroupVariation::Group32Var2):
        return true;
    case(GroupVariation::Group32Var3):
        return true;
    case(GroupVariation::Group32Var4):
        return true;
    case(GroupVariation::Group32Var5):
        return true;
    case(GroupVariation::Group32Var6):
        return true;
    case(GroupVariation::Group32Var7):
        return true;
    case(GroupVariation::Group32Var8):
        return true;
    case(GroupVariation::Group40Var1):
        return true;
    case(GroupVariation::Group40Var2):
        return true;
    case(GroupVariation::Group40Var3):
        return true;
    case(GroupVariation::Group40Var4):
        return true;
    case(GroupVariation::Group42Var1):
        return true;
    case(GroupVariation::Group42Var2):
        return true;
    case(GroupVariation::Group42Var3):
        return true;
    case(GroupVariation::Group42Var4):
        return true;
    case(GroupVariation::Group42Var5):
        return true;
    case(GroupVariation::Group42Var6):
        return true;
    case(GroupVariation::Group42Var7):
        return true;
    case(GroupVariation::Group42Var8):
        return true;
    default:
      return false;
  }
}
bool IsEvent(GroupVariation gv)
{
  switch(gv)
  {
    case(GroupVariation::Group2Var1):
        return true;
    case(GroupVariation::Group2Var2):
        return true;
    case(GroupVariation::Group2Var3):
        return true;
    case(GroupVariation::Group4Var1):
        return true;
    case(GroupVariation::Group4Var2):
        return true;
    case(GroupVariation::Group4Var3):
        return true;
    case(GroupVariation::Group11Var1):
        return true;
    case(GroupVariation::Group11Var2):
        return true;
    case(GroupVariation::Group13Var1):
        return true;
    case(GroupVariation::Group13Var2):
        return true;
    case(GroupVariation::Group22Var1):
        return true;
    case(GroupVariation::Group22Var2):
        return true;
    case(GroupVariation::Group22Var5):
        return true;
    case(GroupVariation::Group22Var6):
        return true;
    case(GroupVariation::Group23Var1):
        return true;
    case(GroupVariation::Group23Var2):
        return true;
    case(GroupVariation::Group23Var5):
        return true;
    case(GroupVariation::Group23Var6):
        return true;
    case(GroupVariation::Group32Var1):
        return true;
    case(GroupVariation::Group32Var2):
        return true;
    case(GroupVariation::Group32Var3):
        return true;
    case(GroupVariation::Group32Var4):
        return true;
    case(GroupVariation::Group32Var5):
        return true;
    case(GroupVariation::Group32Var6):
        return true;
    case(GroupVariation::Group32Var7):
        return true;
    case(GroupVariation::Group32Var8):
        return true;
    case(GroupVariation::Group42Var1):
        return true;
    case(GroupVariation::Group42Var2):
        return true;
    case(GroupVariation::Group42Var3):
        return true;
    case(GroupVariation::Group42Var4):
        return true;
    case(GroupVariation::Group42Var5):
        return true;
    case(GroupVariation::Group42Var6):
        return true;
    case(GroupVariation::Group42Var7):
        return true;
    case(GroupVariation::Group42Var8):
        return true;
    case(GroupVariation::Group43Var1):
        return true;
    case(GroupVariation::Group43Var2):
        return true;
    case(GroupVariation::Group43Var3):
        return true;
    case(GroupVariation::Group43Var4):
        return true;
    case(GroupVariation::Group43Var5):
        return true;
    case(GroupVariation::Group43Var6):
        return true;
    case(GroupVariation::Group43Var7):
        return true;
    case(GroupVariation::Group43Var8):
        return true;
    default:
      return false;
  }
}

}
