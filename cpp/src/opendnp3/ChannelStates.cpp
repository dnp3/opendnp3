
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include <opendnp3/ChannelStates.h>

namespace opendnp3
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string ConvertChannelStateToString(ChannelState aState)
{
	switch(aState) {
	case(CS_CLOSED):
		return "CLOSED";
	case(CS_OPENING):
		return "OPENING";
	case(CS_WAITING):
		return "WAITING";
	case(CS_OPEN):
		return "OPEN";
	case(CS_SHUTDOWN):
		return "SHUTDOWN";
	default:
		return "Undefined state";
	}
}
#endif

}


