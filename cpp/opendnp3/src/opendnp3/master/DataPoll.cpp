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
#include "DataPoll.h"



#include "opendnp3/app/PointClass.h"
#include <openpal/LoggableMacros.h>

#include "opendnp3/app/APDUParser.h"
#include "opendnp3/master/MeasurementHandler.h"

using namespace openpal;

namespace opendnp3
{

/* DataPoll - base class */

DataPoll::DataPoll(Logger& arLogger, ISOEHandler* pHandler_) :
	MasterTaskBase(arLogger),
	pHandler(pHandler_)
{}

bool DataPoll::_OnPartialResponse(const APDUResponseRecord& record)
{
	this->ReadData(record);
	return true;
}

TaskResult DataPoll::_OnFinalResponse(const APDUResponseRecord& record)
{
	this->ReadData(record);
	return TR_SUCCESS;
}

void DataPoll::ReadData(const APDUResponseRecord& record)
{
	MeasurementHandler handler(this->logger, this->pHandler);
	auto res = APDUParser::ParseTwoPass(record.objects, &handler, &logger);
	if(res != APDUParser::Result::OK)
	{
		LOG_BLOCK(LogLevel::Warning, "Error parsing response headers: " << static_cast<int>(res)); // TODO - turn these into strings
	}
}

/* Class Poll */

ClassPoll::ClassPoll(Logger& arLogger, ISOEHandler* pHandler_) :
	DataPoll(arLogger, pHandler_),
	mClassMask(0)
{}

void ClassPoll::Set(uint8_t classMask)
{
	mClassMask = classMask;
}


void ClassPoll::ConfigureRequest(APDURequest& request)
{
	request.SetFunction(FunctionCode::READ);
	auto writer = request.GetWriter();
	if (mClassMask & CLASS_1) writer.WriteHeader(GroupVariationID(60, 2), QualifierCode::ALL_OBJECTS);
	if (mClassMask & CLASS_2) writer.WriteHeader(GroupVariationID(60, 3), QualifierCode::ALL_OBJECTS);
	if (mClassMask & CLASS_3) writer.WriteHeader(GroupVariationID(60, 4), QualifierCode::ALL_OBJECTS);
	if (mClassMask & CLASS_0) writer.WriteHeader(GroupVariationID(60, 1), QualifierCode::ALL_OBJECTS);
}



} //end ns


