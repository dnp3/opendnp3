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

#ifndef __STACK_STATISTICS_H_
#define __STACK_STATISTICS_H_

#include <cstdint>

namespace opendnp3
{
	struct StackStatistics
	{
		StackStatistics() : 
			numTransportRx(0),
			numTransportTx(0),
			numTransportMalformedRx(0), 
			numTransportOverflowRx(0),
			numTransportBadSeq(0)
		{}
		
		/// Number of valid TPDU's received
		uint32_t numTransportRx;

		/// Number of TPDU's transmitted
		uint32_t numTransportTx;	

		/// Number of malformed TPDU's received
		uint32_t numTransportMalformedRx;

		/// Number of times frames were dropped due to TPDU buffer overflow
		uint32_t numTransportOverflowRx;

		/// Number of times frames were dropped due to bad transport sequence numbers
		uint32_t numTransportBadSeq;
	};
}

#endif
