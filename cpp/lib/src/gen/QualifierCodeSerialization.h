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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "opendnp3/gen/QualifierCode.h"
#include <ser4cpp/serialization/LittleEndian.h>

namespace ser4cpp
{
  namespace serializers
  {
    template<>
    inline bool write_one(wseq_t& dest, const opendnp3::QualifierCode& value)
    {
      return UInt8::write_to(dest, opendnp3::QualifierCodeSpec::to_type(value));
    }

    template<>
    inline bool read_one(rseq_t& input, opendnp3::QualifierCode& out)
    {
      uint8_t tempQualifierCode;
      bool result = UInt8::read_from(input, tempQualifierCode);
      out = opendnp3::QualifierCodeSpec::from_type(tempQualifierCode);
      return result;
    }
  }
}
