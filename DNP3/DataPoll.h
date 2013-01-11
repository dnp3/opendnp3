//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __DATA_POLL_H_
#define __DATA_POLL_H_

#include "MasterTaskBase.h"
#include "VtoReader.h"

namespace apl
{
class IDataObserver;
}

namespace apl
{
namespace dnp
{

/**
 * Base class for all data acquistion polls
 */
class DataPoll : public MasterTaskBase
{
public:

	DataPoll(Logger*, IDataObserver*, VtoReader*);

private:

	void ReadData(const APDU&);

	//Implement MasterTaskBase
	TaskResult _OnPartialResponse(const APDU&);
	TaskResult _OnFinalResponse(const APDU&);

	IDataObserver* mpObs;

	VtoReader* mpVtoReader;

};

/** Task that acquires class data from the outstation
*/
class ClassPoll : public DataPoll
{
public:

	ClassPoll(Logger*, IDataObserver*, VtoReader*);

	void Set(int aClassMask);

	//Implement MasterTaskBase
	void ConfigureRequest(APDU& arAPDU);
	virtual std::string Name() const {
		return "Class Poll";
	}

private:

	int mClassMask;

};

}
} //ens ns

/* vim: set ts=4 sw=4: */

#endif
