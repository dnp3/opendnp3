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

#include "WriteHandler.h"

#include <openpal/LoggableMacros.h>

#include <opendnp3/DNPConstants.h>

using namespace openpal;

namespace opendnp3
{

WriteHandler::WriteHandler(openpal::Logger& aLogger) : Loggable(aLogger)	
{}
	
void WriteHandler::_OnIIN(const IterableBuffer<IndexedValue<bool>>& meas)
{	
	IndexedValue<bool> v;
	if(meas.ReadOnlyValue(v)) 
	{
		if(v.index == static_cast<int>(IINBit::DEVICE_RESTART))
		{
			if(v.value)
			{
				ERROR_BLOCK(LogLevel::Warning, "Ignoring device restart IIN write with bit set", SERR_INVALID_IIN_WRITE);
				errors.Set(IINBit::PARAM_ERROR);
			}
			else
			{
				clearMask.Set(IINBit::DEVICE_RESTART);
			}
		}
		else
		{
			ERROR_BLOCK(LogLevel::Warning, "Ignoring IIN write to index: " << v.index, SERR_INVALID_IIN_WRITE);
			errors.Set(IINBit::PARAM_ERROR);
		}			
	}
	else 
	{
		ERROR_BLOCK(LogLevel::Warning, "Ignoring IIN write with a count of: " << meas.Count(), SERR_INVALID_IIN_WRITE);
		errors.Set(IINBit::PARAM_ERROR);
	}	
}

IINField WriteHandler::Process(IINField& indications)
{
	if(errors.Any()) return errors;
	else
	{
		indications &= ~clearMask;
		return IINField::Empty;
	}
}

}


