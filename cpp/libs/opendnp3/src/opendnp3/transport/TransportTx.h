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
#ifndef OPENDNP3_TRANSPORTTX_H
#define OPENDNP3_TRANSPORTTX_H

#include <openpal/logging/Logger.h>
#include <openpal/container/Settable.h>

#include "opendnp3/StackStatistics.h"
#include "opendnp3/link/ITransportSegment.h"
#include "opendnp3/transport/TransportConstants.h"

namespace opendnp3
{

class TransportLayer;

/**
State/validation for the DNP3 transport layer's send channel.
*/
class TransportTx : public ITransportSegment
{

public:

	TransportTx(const openpal::Logger& logger, StackStatistics* pStatistics);

	void Configure(const openpal::ReadOnlyBuffer& output);

	static uint8_t GetHeader(bool fir, bool fin, uint8_t sequence);

	/// -------  IBufferSegment ------------

	virtual bool HasValue() const override final;

	virtual openpal::ReadOnlyBuffer GetSegment() override final;

	virtual bool Advance() override final;

private:

	// A wrapper to the APDU buffer that we're segmenting
	openpal::ReadOnlyBuffer apdu;

	openpal::Settable<openpal::ReadOnlyBuffer> txSegment;

	// Static buffer where we store tpdus that are being transmitted
	uint8_t tpduBuffer[MAX_TPDU_LENGTH];	

	openpal::Logger logger;
	StackStatistics* pStatistics;
	uint8_t sequence;
	uint32_t tpduCount;
};

}

#endif

