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
#include "IntegrationTest.h"

#include <sstream>
#include <thread>

#include "AsyncTestObjectASIO.h"

#include <openpal/IPhysicalLayerAsync.h>

#include <asiodnp3/IMaster.h>
#include <asiodnp3/IChannel.h>
#include <asiodnp3/IOutstation.h>

#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>

#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>
#include <asiopal/UTCTimeSource.h>

#include <asio.hpp>

using namespace std;
using namespace asiopal;
using namespace opendnp3;

IntegrationTest::IntegrationTest(uint16_t aStartPort, size_t aNumPairs, uint16_t aNumPoints, uint32_t filters) :
	M_START_PORT(aStartPort),
	mLog(),	
	mPool(&mLog, filters, std::thread::hardware_concurrency()),
	mMgr(),
	NUM_POINTS(aNumPoints)
{
	this->InitLocalObserver();

	//mFanout.AddObserver(&mLocalFDO); TODO - redesign integration test
	for (size_t i = 0; i < aNumPairs; ++i)
	{
		AddStackPair(filters, aNumPoints);
	}

}

void IntegrationTest::InitLocalObserver()
{
	Transaction tr(&mLocalFDO);
	for (size_t i = 0; i < NUM_POINTS; ++i)
	{
		//mLocalFDO.Update(Binary(false), i); TODO - redesign integration test
		//mLocalFDO.Update(Analog(0.0), i);
		//mLocalFDO.Update(Counter(0), i);
	}
}

void IntegrationTest::ResetObservers()
{
	for (size_t i = 0; i < this->mMasterObservers.size(); ++i)
	{
		mMasterObservers[i]->Reset();
	}
}

bool IntegrationTest::WaitForSameData(TimeDuration aTimeout, bool aDescribeAnyMissingData)
{
	for (size_t i = 0; i < this->mMasterObservers.size(); ++i)
	{
		ComparingDataObserver* pObs = mMasterObservers[i].get();
		if(!pObs->WaitForSameData(aTimeout))
		{
			if(aDescribeAnyMissingData) pObs->DescribeMissingData();
			return false;
		}
	}

	return true;
}

size_t IntegrationTest::IncrementData()
{
	size_t num = 0;

	this->ResetObservers();
	/*
	 * Resource Acquisition Is Initialization (RAII) Pattern.
	 * When the Transaction instance is created, it acquires the resource.
	 * When it is destroyed, it releases the resource.  The scoping using
	 * the {} block forces destruction of the Transaction at the right time.
	*/
	Transaction tr(&mFanout);
	for (uint16_t i = 0; i < NUM_POINTS; ++i)
	{
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

void IntegrationTest::AddStackPair(uint32_t filters, uint16_t aNumPoints)
{
	uint16_t port = M_START_PORT + static_cast<uint16_t>(this->mMasterObservers.size());

	std::shared_ptr<ComparingDataObserver> pMasterFDO(new ComparingDataObserver(&mLocalFDO));
	mMasterObservers.push_back(pMasterFDO);

	auto pClientRoot = new LogRoot(&mLog, "client", filters);
	auto pClientPhys = new PhysicalLayerAsyncTCPClient(*pClientRoot, mPool.GetIOService(), "127.0.0.1", port);
	auto pClient = this->mMgr.CreateChannel(pClientRoot, TimeDuration::Seconds(1), TimeDuration::Seconds(1), pClientPhys);

	auto pServerRoot = new LogRoot(&mLog, "server", filters);
	auto pServerPhys = new PhysicalLayerAsyncTCPServer(*pServerRoot, mPool.GetIOService(), "127.0.0.1", port);
	auto pServer = this->mMgr.CreateChannel(pServerRoot, TimeDuration::Seconds(1), TimeDuration::Seconds(1), pServerPhys);

	/*
	 * Add a Master instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		MasterStackConfig cfg;
		cfg.app.RspTimeout = TimeDuration::Seconds(10);
		cfg.master.IntegrityRate = TimeDuration::Min();
		cfg.master.EnableUnsol = true;
		cfg.master.DoUnsolOnStartup = true;
		auto pMaster = pClient->AddMaster("master", pMasterFDO.get(), asiopal::UTCTimeSource::Inst(), cfg);
		pMaster->Enable();
	}

	/*
	 * Add a Outstation instance.  The code is wrapped in braces so that we can
	 * re-use the 'cfg' variable name.
	 */
	{
		OutstationStackConfig cfg(DatabaseTemplate(aNumPoints, aNumPoints, aNumPoints, aNumPoints));
		cfg.app.RspTimeout = TimeDuration::Seconds(10);
		cfg.outstation.mDisableUnsol = false;
		cfg.outstation.mUnsolPackDelay = TimeDuration::Zero();
		auto pOutstation = pServer->AddOutstation("outstation", &mCmdHandler, NullTimeWriteHandler::Inst(), cfg);
		this->mFanout.AddObserver(pOutstation->GetLoader());
		pOutstation->Enable();
	}

}



