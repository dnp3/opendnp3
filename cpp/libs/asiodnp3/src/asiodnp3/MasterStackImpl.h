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
#ifndef ASIODNP3_MASTERSTACKIMPL_H
#define ASIODNP3_MASTERSTACKIMPL_H

#include "MasterBase.h"
#include "IStackLifecycle.h"

#include <opendnp3/link/ILinkSession.h>
#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/Master.h>
#include <opendnp3/transport/TransportStack.h>

#include <secauth/master/IMasterUser.h>

#include <openpal/executor/IUTCTimeSource.h>
#include <openpal/crypto/ICryptoProvider.h>

namespace asiodnp3
{


/** @section desc A stack object for a master */
class MasterStackImpl : public MasterBase
{
public:

	// constructor for normal DNP3
	MasterStackImpl(
		const char* id,
		openpal::LogRoot& root,
	    asiopal::ASIOExecutor& executor,
		opendnp3::ISOEHandler& SOEHandler,	    
		opendnp3::IMasterApplication& application,
		const opendnp3::MasterStackConfig& config,		
		IStackLifecycle& lifecycle,
		opendnp3::ITaskLock& taskLock
	);

	//constructor for secure DNP3
	MasterStackImpl(
		const char* id,
		openpal::LogRoot& root,
		asiopal::ASIOExecutor& executor,
		opendnp3::ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		const opendnp3::MasterStackConfig& config,
		IStackLifecycle& lifecycle,
		opendnp3::ITaskLock& taskLock,
		secauth::IMasterUser& user,
		openpal::ICryptoProvider& crypto
	);

	virtual bool Enable() override final;

	virtual bool Disable() override final;

	virtual void Shutdown() override final;

	virtual opendnp3::StackStatistics GetStackStatistics() override final;		

	virtual opendnp3::ICommandProcessor* GetCommandProcessor()  override final;

	// ------- Periodic scan API ---------

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::vector<Header>& headers, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::function<void(opendnp3::HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual opendnp3::MasterScan AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual opendnp3::MasterScan AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual opendnp3::MasterScan AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId) override final;

	// ------- Adhoc scan API ---------

	virtual void Scan(const std::vector<Header>& headers, opendnp3::ITaskCallback* pCallback, int userId) override final;
	
	virtual void Scan(const std::function<void(opendnp3::HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual void ScanAllObjects(opendnp3::GroupVariationID gvId, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual void ScanClasses(const opendnp3::ClassField& field, opendnp3::ITaskCallback* pCallback, int userId) override final;

	virtual void ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, opendnp3::ITaskCallback* pCallback, int userId) override final;

	// ------- Other adhoc methods -------

	virtual void Write(const opendnp3::TimeAndInterval& value, uint16_t index, opendnp3::ITaskCallback* pCallback, int userId)  override final;

	// ------- implement ILinkBind ---------

	virtual void SetLinkRouter(opendnp3::ILinkRouter& router) override final;

	virtual opendnp3::ILinkSession& GetLinkContext() override final;


protected:

	static std::function<void(opendnp3::HeaderWriter&)> ConvertToLambda(const std::vector<Header>& headers);	

	openpal::LogRoot root;	
	opendnp3::StackStatistics statistics;
	IStackLifecycle* pLifecycle;
	opendnp3::TransportStack stack;
	std::unique_ptr<opendnp3::MContext> mcontext;
	opendnp3::Master master;
};

}

#endif

