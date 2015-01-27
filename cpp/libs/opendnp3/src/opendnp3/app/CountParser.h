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
#ifndef OPENDNP3_COUNTPARSER_H
#define OPENDNP3_COUNTPARSER_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/logging/Logger.h>
#include <openpal/logging/LogMacros.h>
#include <openpal/util/Uncopyable.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/ErrorCodes.h"

#include "opendnp3/app/ParseResult.h"
#include "opendnp3/app/NumParser.h"
#include "opendnp3/app/ParserSettings.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/app/IAPDUHandler.h"
#include "opendnp3/app/LazyIterable.h"

namespace opendnp3
{

class CountParser : private openpal::PureStatic
{
public:	

	static ParseResult ParseHeader(openpal::ReadBufferView& buffer, const NumParser& numparser, const ParserSettings& settings, const HeaderRecord& record, openpal::Logger* pLogger, IAPDUHandler* pHandler);

private:

	static ParseResult ParseCountOfObjects(openpal::ReadBufferView& buffer, const HeaderRecord& record, uint16_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler);

	template <class Descriptor>
	static ParseResult ParseCountOf(openpal::ReadBufferView& buffer, const HeaderRecord& record, uint32_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler);
};

template <class Descriptor>
ParseResult CountParser::ParseCountOf(openpal::ReadBufferView& buffer, const HeaderRecord& record, uint32_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler)
{
	uint32_t size = count * Descriptor::Size();
	if (buffer.Size() < size)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified objects");
		return ParseResult::NOT_ENOUGH_DATA_FOR_OBJECTS;
	}
	else
	{
		if(pHandler)
		{
			auto collection = CreateLazyIterable<Descriptor>(buffer, count, [](openpal::ReadBufferView & buffer, uint32_t)
			{
				return Descriptor::Read(buffer);
			});
			pHandler->OnCountOf(record, collection);
		}
		buffer.Advance(size);
		return ParseResult::OK;
	}
}

}

#endif
