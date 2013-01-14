
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "IntegrationTest.h"

#include <sstream>

#include <APL/PhysicalLayerFactory.h>
#include <APL/IPhysicalLayerAsync.h>

#include <APLTestTools/AsyncTestObjectASIO.h>

#include <DNP3/MasterStackConfig.h>
#include <DNP3/SlaveStackConfig.h>

#include <DNP3/MasterStack.h>
#include <DNP3/SlaveStack.h>

#include <boost/asio.hpp>

#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>


using namespace std;
using namespace apl;
using namespace apl::dnp;

IntegrationTest::IntegrationTest(Logger* apLogger, FilterLevel aLevel, boost::uint16_t aStartPort, size_t aNumPairs, size_t aNumPoints) :
	Loggable(apLogger),
	M_START_PORT(aStartPort),
	mManager(apLogger),
	NUM_POINTS(aNumPoints)
{
	this->InitLocalObserver();

	mFanout.AddObserver(&mLocalFDO);
	for (size_t i = 0; i < aNumPairs; ++i) {
		AddStackPair(aLevel, aNumPoints);
	}

}

void IntegrationTest::InitLocalObserver()
{
	Transaction tr(&mLocalFDO);
	for (size_t i = 0; i < NUM_POINTS; ++i) {
		mLocalFDO.Update(Binary(false), i);
		mLocalFDO.Update(Analog(0.0), i);
		mLocalFDO.Update(Counter(0), i);
	}
}

void IntegrationTest::ResetObservers()
{
	for (size_t i = 0; i < this->mMasterObservers.size(); ++i) {
		mMasterObservers[i]->Reset();
	}
}

bool IntegrationTest::WaitForSameData(millis_t aTimeout, bool aDescribeAnyMissingData)
{
	LOG_BLOCK(LEV_EVENT, "Wait for same data");

	for (size_t i = 0; i < this->mMasterObservers.size(); ++i) {
		ComparingDataObserver* pObs = mMasterObservers[i].get();
		if(!pObs->WaitForSameData(aTimeout)) {
			if(aDescribeAnyMissingData) pObs->DescribeMissingData();
			return false;
		}
	}

	return true;
}

size_t IntegrationTest::IncrementData()
{
	LOG_BLOCK(LEV_EVENT, "Incrementing data");

	size_t num = 0;

	this->ResetObservers();
	/*
	 * Resource Acquisition Is Initialization (RAII) Pattern.
	 * When the Transaction instance is created, it acquires the resource.
	 * When it is destroyed, it releases the resource.  The scoping using
	 * the {} block forces destruction of the Transaction at the right time.
	*/
	Transaction tr(&mFanout);
	for (size_t i = 0; i < NUM_POINTS; ++i) {
		mFanout.Update(this->Next(mLocalFDO.mBinaryMap[i]), i);
		mFanout.Update(this->Next(mLocalFDO.mAnalogMap[i]), i);
		mFanout.Update(this->Next(mLocalFDO.mCounterMap[i]), i);
		num += 3;
	}
	return num;
}

Binary IntegrationTest::Next(const Binary& arPoint)
{
	Binary point(!arPoint.GetValue(), arPoint.GetQuality());
	return point;
}

Analog IntegrationTest::Next(const Analog& arPoint)
{
	Analog point(arPoint.GetValue() + 1, arPoint.GetQuality());
	return point;
}

Counter IntegrationTest::Next(const Counter& arPoint)
{
	Counter point(arPoint.GetValue() + 1, arPoint.GetQuality());
	return point;
}

void IntegrationTest::AddStackPair(FilterLevel aLevel, size_t aNumPoints)
{
	boost::uint16_t port = M_START_PORT + static_cast<boost::uint16_t>(this->mMasterObservers.size());

	ostringstream oss;
	oss << "Port: " << port;
	std::string client = oss.str() + " Client ";
	std::string server = oss.str() + " Server ";

	std::shared_ptr<ComparingDataObserver> pMasterFDO(new ComparingDataObserver(mpLogger->GetSubLogger(client), &mLocalFDO));
	mMasterObservers.push_back(pMasterFDO);

	PhysLayerSettings s(aLevel, 1000);
	this->mManager.AddTCPClient(client, s, "127.0.0.1", port);
	this->mManager.AddTCPServer(server, s, "127.0.0.1", port);

	/*
	 * Add a Master instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		MasterStackConfig cfg;
		cfg.app.RspTimeout = 20000;
		cfg.master.IntegrityRate = -1;
		cfg.master.EnableUnsol = true;
		cfg.master.DoUnsolOnStartup = true;
		cfg.master.UnsolClassMask = PC_ALL_EVENTS;
		this->mManager.AddMaster(client, client, aLevel, pMasterFDO.get(), cfg);
	}

	/*
	 * Add a Slave instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		SlaveStackConfig cfg;
		cfg.app.RspTimeout = 20000;
		cfg.slave.mDisableUnsol = false;
		cfg.slave.mUnsolPackDelay = 0;
		cfg.device = DeviceTemplate(aNumPoints, aNumPoints, aNumPoints);
		IDataObserver* pObs = this->mManager.AddSlave(server, server, aLevel, &mCmdAcceptor, cfg);
		this->mFanout.AddObserver(pObs);
	}

}

/* vim: set ts=4 sw=4: */

