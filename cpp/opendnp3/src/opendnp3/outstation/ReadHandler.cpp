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

#include "ReadHandler.h"

namespace opendnp3
{

ReadHandler::ReadHandler(openpal::Logger& logger, StaticResponseContext* pStaticContext_, EventResponseContext* pEventResponseContext) :
	APDUHandlerBase(logger),
	pStaticContext(pStaticContext_),
	pEventContext(pEventResponseContext)
{

}

void ReadHandler::_AllObjects(const GroupVariationRecord& record)
{	

	switch (record.enumeration)
	{
		// static objects
		case(GroupVariation::Group60Var1) :
		case(GroupVariation::Group1Var0) :
		case(GroupVariation::Group1Var2) :
		case(GroupVariation::Group10Var0) :
		case(GroupVariation::Group10Var2) :
		case(GroupVariation::Group20Var0) :
		case(GroupVariation::Group20Var1) :
		case(GroupVariation::Group20Var2) :
		case(GroupVariation::Group20Var5) :
		case(GroupVariation::Group20Var6) :
		case(GroupVariation::Group21Var0) :
		case(GroupVariation::Group21Var1) :
		case(GroupVariation::Group21Var2) :
		case(GroupVariation::Group21Var5) :
		case(GroupVariation::Group21Var6) :
		case(GroupVariation::Group21Var9) :
		case(GroupVariation::Group21Var10) :
		case(GroupVariation::Group30Var0) :
		case(GroupVariation::Group30Var1) :
		case(GroupVariation::Group30Var2) :
		case(GroupVariation::Group30Var3) :
		case(GroupVariation::Group30Var4) :
		case(GroupVariation::Group30Var5) :
		case(GroupVariation::Group30Var6) :
		case(GroupVariation::Group40Var0) :
		case(GroupVariation::Group40Var1) :
		case(GroupVariation::Group40Var2) :
		case(GroupVariation::Group40Var3) :
		case(GroupVariation::Group40Var4) :
			errors |= pStaticContext->QueueReadAllObjects(record);
			break;
		default:
			errors.Set(IINBit::FUNC_NOT_SUPPORTED);
			break;
	}
}

void ReadHandler::_OnRangeRequest(const GroupVariationRecord& record, const StaticRange& range)
{
	/*
	switch (pStaticContext->QueueReadRange(record, range))
	{
		case(QueueResult::FULL):
		case(QueueResult::SUCCESS):
			break;
		case(QueueResult::OBJECT_UNDEFINED):
			errors |= IINField(IINBit::FUNC_NOT_SUPPORTED);
			break;
		case(QueueResult::OUT_OF_RANGE):
			errors |= IINField(IINBit::PARAM_ERROR);
			break;
	}
	*/
}




}


