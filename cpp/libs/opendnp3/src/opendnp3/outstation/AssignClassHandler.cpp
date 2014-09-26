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

AssignClassHandler::AssignClassHandler(openpal::Logger& logger, IOutstationApplication& application, Database& database) : 
	APDUHandlerBase(logger),
	classIsValid(false),
	clazz(PointClass::Class0),
	pApplication(&application),
	pDatabase(&database)
{

}

void AssignClassHandler::_AllObjects(const HeaderRecord& record)
{
	if (classIsValid)
	{
		classIsValid = false;
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

void AssignClassHandler::_OnRangeRequest(const HeaderRecord& record, const StaticRange& range)
{
	if (classIsValid)
	{
		classIsValid = false;
		switch (record.enumeration)
		{
		case(GroupVariation::Group1Var0) :
			this->ProcessAssignment(AssignClassType::BinaryInput, clazz, range);
			break;
		case(GroupVariation::Group3Var0) :
			this->ProcessAssignment(AssignClassType::DoubleBinaryInput, clazz, range);
			break;
		case(GroupVariation::Group10Var0) :
			this->ProcessAssignment(AssignClassType::BinaryOutputStatus, clazz, range);
			break;
		case(GroupVariation::Group20Var0) :
			this->ProcessAssignment(AssignClassType::Counter, clazz, range);
			break;
		case(GroupVariation::Group21Var0) :
			this->ProcessAssignment(AssignClassType::FrozenCounter, clazz, range);
			break;
		case(GroupVariation::Group30Var0) :
			this->ProcessAssignment(AssignClassType::AnalogInput, clazz, range);
			break;
		case(GroupVariation::Group40Var0) :
			this->ProcessAssignment(AssignClassType::AnalogOutputStatus, clazz, range);
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
	if (pDatabase->AssignClass(type, clazz, range))
	{
		pApplication->RecordClassAssignment(type, clazz, range.start, range.stop);
	}
	else
	{
		errors.SetBit(IINBit::PARAM_ERROR);
	}
	
}

void AssignClassHandler::RecordClass(GroupVariation gv)
{
	switch (gv)
	{
	case(GroupVariation::Group60Var1) :
		clazz = PointClass::Class0;
		classIsValid = true;
		break;
	case(GroupVariation::Group60Var2) :
		clazz = PointClass::Class1;
		classIsValid = true;
		break;
	case(GroupVariation::Group60Var3) :
		clazz = PointClass::Class2;
		classIsValid = true;
		break;
	case(GroupVariation::Group60Var4) :
		clazz = PointClass::Class3;
		classIsValid = true;
		break;
	default:		
		classIsValid = false;
		break;
	}
}

}

