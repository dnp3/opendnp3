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
#ifndef __WRITE_HANDLER_H_
#define __WRITE_HANDLER_H_

#include "HeaderHandlerBase.h"
#include "IINField.h"
#include "SettableOnce.h"

#include <opendnp3/ITimeWriteHandler.h>

#include <openpal/Loggable.h>

namespace opendnp3
{

/**
 * Dedicated class for processing response data in the master.
 */
class WriteHandler : public HeaderHandlerBase, private openpal::Loggable
{
	public:
	
	WriteHandler(openpal::Logger& aLogger);		


	virtual void _OnIIN(const IterableBuffer<IndexedValue<bool>>& meas) final;
	virtual void _OnCountOf(const IterableBuffer<Group50Var1>& times) final;

	template <class TimeFun>
	IINField Process(IINField& writeIIN, const TimeFun&);

	private:
	
	SettableOnce<Group50Var1> timeWrite;
	IINField clearMask;
};

template <class TimeFun>
IINField WriteHandler::Process(IINField& indications, const TimeFun& fun)
{
	if(errors.Any()) return errors;
	else
	{
		if(timeWrite.IsSet())
		{
			if(indications.IsSet(IINBit::NEED_TIME)) 
			{
				clearMask.Set(IINBit::NEED_TIME);
				indications &= ~clearMask;
				fun(timeWrite.Get());
				return IINField::Empty;
			}
			else return IINField(IINBit::PARAM_ERROR); // asked to write time, but we're not asking for it			
		}
		else
		{
			indications &= ~clearMask;
			return IINField::Empty;
		}
	}
}

}

/* vim: set ts=4 sw=4: */

#endif

