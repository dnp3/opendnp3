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

#include "DNPConstants.h"

using namespace openpal;

namespace opendnp3
{

WriteHandler::WriteHandler(openpal::Logger& aLogger, ITimeWriteHandler* pTimeWriteHandler_, IINField* pWriteIIN_) : 
	Loggable(aLogger),
	pTimeWriteHandler(pTimeWriteHandler_),
	pWriteIIN(pWriteIIN_),
	wroteTime(false),
	wroteIIN(false)	
{}
	
void WriteHandler::_OnIIN(const IterableBuffer<IndexedValue<bool, uint16_t>>& meas)
{	
	IndexedValue<bool, uint16_t> v;
	if(meas.ReadOnlyValue(v)) 
	{
		if (wroteIIN) errors.Set(IINBit::PARAM_ERROR);
		else
		{			
			if (v.index == static_cast<int>(IINBit::DEVICE_RESTART))
			{
				if (v.value) errors.Set(IINBit::PARAM_ERROR);
				else 
				{
					wroteIIN = true;
					pWriteIIN->Clear(IINBit::DEVICE_RESTART);
				}
			}
			else errors.Set(IINBit::PARAM_ERROR);
		}		
	}
	else errors.Set(IINBit::PARAM_ERROR);	
}

void WriteHandler::_OnCountOf(const IterableBuffer<Group50Var1>& times)
{
	if (wroteTime) errors.Set(IINBit::PARAM_ERROR);
	else
	{		
		if (pWriteIIN->IsSet(IINBit::NEED_TIME))
		{
			Group50Var1 time;
			if (times.ReadOnlyValue(time))
			{
				wroteTime = true;
				pWriteIIN->Clear(IINBit::NEED_TIME);
				pTimeWriteHandler->WriteAbsoluteTime(UTCTimestamp(time.time));
			}
			else errors.Set(IINBit::PARAM_ERROR);
		}
		else
		{
			errors.Set(IINBit::PARAM_ERROR);		
		}
	}
}

}


