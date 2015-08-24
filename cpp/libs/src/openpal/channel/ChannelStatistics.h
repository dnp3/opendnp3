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

#ifndef OPENPAL_CHANNELSTATISTICS_H
#define OPENPAL_CHANNELSTATISTICS_H

#include <cstdint>

namespace openpal
{
	struct ChannelStatistics
	{
		ChannelStatistics() : numOpen(0), numOpenFail(0), numClose(0), numBytesRx(0), numBytesTx(0)
		{}

		/// The number of times the channel has successfully opened
		uint32_t numOpen;

		/// The number of times the channel has failed to open
		uint32_t numOpenFail;

		/// The number of times the channel has closed either due to user intervention or an error
		uint32_t numClose;

		/// The number of bytes received
		uint32_t numBytesRx;

		/// The number of bytes transmitted
		uint32_t numBytesTx;
	};
}

#endif
