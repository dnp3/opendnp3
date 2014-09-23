/**
* Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
* more contributor license agreements. See the NOTICE file distributed
* with this work for additional information regarding copyright ownership.
* Green Energy Corp licenses this file to you under the Apache License,
* Version 2.0 (the "License"); you may not use this file except in
* compliance with the License.  You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* This project was forked on 01/01/2013 by Automatak, LLC and modifications
* may have been made to this file. Automatak, LLC licenses these modifications
* to you under the terms of the License.
*/

#ifndef OPENDNP3_LINKCHANNELSTATISTICS_H
#define OPENDNP3_LINKCHANNELSTATISTICS_H

#include <openpal/channel/ChannelStatistics.h>

namespace opendnp3
{
	struct LinkChannelStatistics : openpal::ChannelStatistics
	{
		LinkChannelStatistics() : numCrcError(0), numLinkFrameTx(0), numLinkFrameRx(0), numBadLinkFrameRx(0)
		{}
		
		/// Number of frames discared due to CRC errors
		uint32_t numCrcError;

		/// Number of frames transmitted
		uint32_t numLinkFrameTx;

		/// Number of frames received
		uint32_t numLinkFrameRx;
		
		/// Number of frames detected with bad / malformed contents
		uint32_t numBadLinkFrameRx;		
	};
}

#endif
