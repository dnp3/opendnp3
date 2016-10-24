/*
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

namespace opendnp3
{

/**
* Counters for the channel and the DNP3 link layer
*/
struct LinkChannelStatistics
{	
	/// The number of times the channel has successfully opened
	size_t numOpen = 0;

	/// The number of times the channel has failed to open
	size_t numOpenFail = 0;

	/// The number of times the channel has closed either due to user intervention or an error
	size_t numClose = 0;

	/// The number of bytes received
	size_t numBytesRx = 0;

	/// The number of bytes transmitted
	size_t numBytesTx = 0;

	/// Number of frames discared due to CRC errors
	size_t numCrcError = 0;

	/// Number of frames transmitted
	size_t numLinkFrameTx = 0;

	/// Number of frames received
	size_t numLinkFrameRx = 0;

	/// Number of frames detected with bad / malformed contents
	size_t numBadLinkFrameRx = 0;
};
}

#endif
