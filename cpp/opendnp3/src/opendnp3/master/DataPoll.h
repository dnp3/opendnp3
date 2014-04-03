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
#ifndef __DATA_POLL_H_
#define __DATA_POLL_H_

#include "MasterTaskBase.h"

#include "ISOEHandler.h"

namespace opendnp3
{

/**
 * Base class for all data acquistion polls
 */
class DataPoll : public MasterTaskBase
{
public:

	DataPoll(openpal::Logger&, ISOEHandler* pHandler_);

private:

	void ReadData(const APDUResponseRecord& record);

	//Implement MasterTaskBase
	bool _OnPartialResponse(const APDUResponseRecord& record) override;
	TaskResult _OnFinalResponse(const APDUResponseRecord& record) override;

	ISOEHandler* pHandler;

};

/** Task that acquires class data from the outstation
*/
class ClassPoll : public DataPoll
{
public:

	ClassPoll(openpal::Logger&, ISOEHandler* pHandler_);

	void Set(uint8_t classMask);

	//Implement MasterTaskBase

	void ConfigureRequest(APDURequest& request);

	virtual char const* Name() const
	{
		return "Class Poll";
	}

private:

	int mClassMask;

};

} //ens ns



#endif
