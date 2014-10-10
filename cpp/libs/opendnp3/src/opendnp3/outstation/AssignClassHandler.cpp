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

#include "AssignClassHandler.h"

#include "opendnp3/app/APDUHandlerBase.h"

#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/IOutstationApplication.h"

#include <openpal/logging/Logger.h>

namespace opendnp3
{

AssignClassHandler::AssignClassHandler(openpal::Logger& logger, openpal::IExecutor& executor, IOutstationApplication& application, Database& database) :
	APDUHandlerBase(logger),
	classHeader(-1),
	clazz(PointClass::Class0),
	pExecutor(&executor),
	pApplication(&application),
	pDatabase(&database)
{

}

void AssignClassHandler::_AllObjects(const HeaderRecord& record)
{
	if (IsExpectingAssignment())
	{		
		switch (record.enumeration)
		{
			case(GroupVariation::Group1Var0) :
				this->ProcessAssignment(AssignClassType::BinaryInput, clazz, pDatabase->FullRange<Binary>());
				break;
			case(GroupVariation::Group3Var0) :
				this->ProcessAssignment(AssignClassType::DoubleBinaryInput, clazz, pDatabase->FullRange<DoubleBitBinary>());
				break;
			case(GroupVariation::Group10Var0) :
				this->ProcessAssignment(AssignClassType::BinaryOutputStatus, clazz, pDatabase->FullRange<BinaryOutputStatus>());
				break;
			case(GroupVariation::Group20Var0) :
				this->ProcessAssignment(AssignClassType::Counter, clazz, pDatabase->FullRange<Counter>());
				break;
			case(GroupVariation::Group21Var0) :
				this->ProcessAssignment(AssignClassType::FrozenCounter, clazz, pDatabase->FullRange<FrozenCounter>());
				break;
			case(GroupVariation::Group30Var0) :
				this->ProcessAssignment(AssignClassType::AnalogInput, clazz, pDatabase->FullRange<Analog>());
				break;
			case(GroupVariation::Group40Var0) :
				this->ProcessAssignment(AssignClassType::AnalogOutputStatus, clazz, pDatabase->FullRange<AnalogOutputStatus>());
				break;
			default:	
				errors.SetBit(IINBit::FUNC_NOT_SUPPORTED);
				break;
		}		
	}
	else
	{	
		this->RecordClass(record.enumeration);
	}
}

bool AssignClassHandler::IsExpectingAssignment()
{
	int32_t current = static_cast<int32_t>(this->GetCurrentHeader());

	if (current > 0 && ((current - 1) == this->classHeader))
	{ 
		this->classHeader = -1;
		return true;
	}
	else
	{
		return false;
	}
}

void AssignClassHandler::_OnRangeRequest(const HeaderRecord& record, const Range& range)
{
	if (IsExpectingAssignment())
	{
		switch (record.enumeration)
		{
		case(GroupVariation::Group1Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<Binary>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::BinaryInput, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group3Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<DoubleBitBinary>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::DoubleBinaryInput, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group10Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<BinaryOutputStatus>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::BinaryOutputStatus, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group20Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<Counter>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::Counter, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group21Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<FrozenCounter>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::FrozenCounter, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group30Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<Analog>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::AnalogInput, clazz, staticrange);
            }
			break;
		case(GroupVariation::Group40Var0) :
            {
            StaticRange staticrange(pDatabase->FullRange<AnalogOutputStatus>());
            staticrange.ClipTo(range);
			this->ProcessAssignment(AssignClassType::AnalogOutputStatus, clazz, staticrange);
            }
			break;
		default:
			errors.SetBit(IINBit::FUNC_NOT_SUPPORTED);
			break;
		}
	}	
	else
	{
		errors.SetBit(IINBit::FUNC_NOT_SUPPORTED);
	}
}

void AssignClassHandler::ProcessAssignment(AssignClassType type, PointClass clazz, const StaticRange& range)
{
	if (!range.IsClipped() && pDatabase->AssignClass(type, clazz, range))
	{
		auto start = range.start;
		auto stop = range.stop;
		auto pApplication = this->pApplication;
		auto callback = [pApplication, start, stop, clazz, type]()
		{
			pApplication->RecordClassAssignment(type, clazz, start, stop);
		};
		pExecutor->PostLambda(callback);
	}
	else
	{
		errors.SetBit(IINBit::PARAM_ERROR);
	}
	
}

void AssignClassHandler::RecordClass(GroupVariation gv)
{
	classHeader = this->GetCurrentHeader();

	switch (gv)
	{
		case(GroupVariation::Group60Var1) :
			clazz = PointClass::Class0;				
			break;
		case(GroupVariation::Group60Var2) :
			clazz = PointClass::Class1;		
			break;
		case(GroupVariation::Group60Var3) :
			clazz = PointClass::Class2;		
			break;
		case(GroupVariation::Group60Var4) :
			clazz = PointClass::Class3;		
			break;
		default:		
			classHeader = -1;
			break;
	}
}

}

