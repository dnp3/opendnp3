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

#include "APDULogging.h"

#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDUParser.h"

namespace opendnp3
{
namespace logging
{

void ParseAndLogRequestTx(openpal::Logger* pLogger, const openpal::ReadOnlyBuffer& output)
{

#ifndef OPENPAL_STRIP_LOGGING

	if (pLogger && pLogger->IsEnabled(flags::APP_HEADER_TX))
	{
		APDURecord record;
		auto result = APDUHeaderParser::ParseRequest(output, record, pLogger);
		if (result == APDUHeaderParser::Result::OK)
		{
			FORMAT_LOGGER_BLOCK(pLogger, flags::APP_HEADER_TX,
				"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
				record.control.FIN,
				record.control.FIN,
				record.control.CON,
				record.control.UNS,
				record.control.SEQ,
				FunctionCodeToString(record.function));

			if (pLogger->IsEnabled(flags::APP_OBJECT_TX))
			{
				auto expectsContents = record.function != FunctionCode::READ;
				APDUParser::ParseTwoPass(record.objects, nullptr, pLogger, APDUParser::Context(expectsContents, flags::APP_OBJECT_TX));
			}
		}
	}

#endif

}
	
}
}

