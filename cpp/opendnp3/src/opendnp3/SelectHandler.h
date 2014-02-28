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
#ifndef __SELECT_HANDLER_H_
#define __SELECT_HANDLER_H_

#include "APDUHandlerBase.h"

#include <openpal/StaticBuffer.h>
#include <opendnp3/ICommandHandler.h>

#include "APDUResponse.h"
#include "StaticSizeConfiguration.h"


namespace opendnp3
{

	class SelectHandler : public APDUHandlerBase
	{
	public:

		SelectHandler(ICommandHandler* pCommandHandler_, uint16_t maxCommands_, APDUResponse& response_);

		virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas);
		virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas);
		virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas);
		virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas);
		virtual void _OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas);

	private:

		bool failure;
		ICommandHandler* pCommandHandler;
		uint32_t numCommands;
		uint16_t maxCommands;
		ObjectWriter writer;
	};

}


#endif