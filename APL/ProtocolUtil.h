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
#ifndef __PROTOCOL_UTIL_H_
#define __PROTOCOL_UTIL_H_

#include <stddef.h>


namespace apl
{

// Given a buffer and max packet size, calculcate the
// maximum number of packets the buffer can hold
size_t CalcMaxPackets(size_t aBuffer, size_t aPayload);


// Given a buffer and max packet size, calculcate the
// size of the last packet.
size_t CalcLastPacketSize(size_t aBuffer, size_t aPayload);


}

#endif

