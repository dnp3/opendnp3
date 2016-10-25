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
#ifndef OPENDNP3_STACKSTATISTICS_H
#define OPENDNP3_STACKSTATISTICS_H

#include <cstdint>

namespace opendnp3
{

/**
* Statistics related to both a master or outstation session
*/
struct StackStatistics
{
	struct Link
	{
		/// number of unexpected frames
		size_t numUnexpectedFrame = 0;

		/// frames received w/ wrong master bit
		size_t numBadMasterBit = 0;

		/// frames received for an unknown destination
		size_t numUnknownDestination = 0;

		/// frames received for an unknown source
		size_t numUnknownSource = 0;
	};

	struct Transport
	{
		struct Rx
		{
			/// Number of valid TPDU's received
			size_t numTransportRx = 0;

			/// Number of TPDUs dropped due to malformed contents
			size_t numTransportErrorRx = 0;

			/// Number of times received data was too big for reassembly buffer
			size_t numTransportBufferOverflow = 0;

			/// number of times transport buffer is discard due to new FIR
			size_t numTransportDiscard = 0;

			/// number of segments ignored due to bad FIR/FIN or SEQ
			size_t numTransportIgnore = 0;
		};

		struct Tx
		{
			/// Number of TPDUs transmitted
			size_t numTransportTx = 0;
		};

		Transport() = default;
		Transport(const Rx& rx, const Tx& tx) : rx(rx), tx(tx) {}

		Rx rx;
		Tx tx;
	};

	StackStatistics() = default;

	StackStatistics(const Link& link, const Transport& transport) :
		link(link),
		transport(transport)
	{

	}

	Link link;
	Transport transport;
};

}

#endif
