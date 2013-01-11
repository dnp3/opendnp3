//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __TRANSPORT_CONSTANTS_H_
#define __TRANSPORT_CONSTANTS_H_


#include "DNPConstants.h"

namespace apl
{
namespace dnp
{

enum TransportHeader {
	TL_HDR_FIN = 0x80,
	TL_HDR_FIR = 0x40,
	TL_HDR_SEQ = 0x3F
};

// The macro MACRO_DNP_MAX_FRAG_SIZE controls the maximum size of application layer frames
const size_t TL_MAX_TPDU_LENGTH = 250;
const size_t TL_MAX_TPDU_PAYLOAD = 249;

}
}

#endif
