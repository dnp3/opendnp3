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
#ifndef ASIODNP3_MASTERSTACKBASE_H
#define ASIODNP3_MASTERSTACKBASE_H

#include <opendnp3/link/ILinkSession.h>
#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/Master.h>
#include <opendnp3/transport/TransportStack.h>

#include <asiopal/ASIOExecutor.h>

#include "IStackLifecycle.h"
#include "IMaster.h"
#include "ILinkBind.h"
#include "Conversions.h"

namespace asiodnp3
{

template <class Interface>
class MasterStackBase : public Interface, public ILinkBind
{
public:
	
	MasterStackBase(
		const char* id,
		openpal::LogRoot& root_,
		asiopal::ASIOExecutor& executor,		
		const opendnp3::MasterStackConfig& config,
		IStackLifecycle& lifecycle		
		) : 
			root(root_, id),
			pLifecycle(&lifecycle),
			stack(root, &executor, config.master.maxRxFragSize, &statistics, config.link)
	{
	
	}
	
	virtual bool Enable() override final
	{
		return pLifecycle->EnableRoute(&stack.link);
	}

	virtual bool Disable() override final
	{
		return pLifecycle->DisableRoute(&stack.link);
	}

	virtual void Shutdown() override final
	{
		return pLifecycle->Shutdown(&stack.link, this);
	}

	virtual opendnp3::StackStatistics GetStackStatistics() override final
	{
		auto get = [this]() { return this->statistics; };
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::StackStatistics>(get);
	}	

	// ------- Periodic scan API ---------

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::vector<Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto func = ConvertToLambda(headers);
		return this->AddScan(period, func, config);
	}

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::function<void(opendnp3::HeaderWriter&)>& builder, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, period, builder, config]() { return this->GetContext().AddScan(period, builder, config); };
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, period, config]() { return this->GetContext().AddAllObjectsScan(gvId, period, config); };
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, field, period, config]() { return this->GetContext().AddClassScan(field, period, config); };
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, start, stop, period, config]() { return this->GetContext().AddRangeScan(gvId, start, stop, period, config); };
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	// ------- Adhoc scan API ---------

	virtual void Scan(const std::vector<Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto func = ConvertToLambda(headers);
		this->Scan(func, config);
	}
	
	virtual void Scan(const std::function<void(opendnp3::HeaderWriter&)>& builder, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, builder, config]() { this->GetContext().Scan(builder, config); };
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanAllObjects(opendnp3::GroupVariationID gvId, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, config]() { this->GetContext().ScanAllObjects(gvId, config); };
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanClasses(const opendnp3::ClassField& field, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, field, config]() { this->GetContext().ScanClasses(field, config); };
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, start, stop, config]() { this->GetContext().ScanRange(gvId, start, stop, config); };
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	// ------- Other adhoc methods -------

	virtual void Write(const opendnp3::TimeAndInterval& value, uint16_t index, const opendnp3::TaskConfig& config)  override final
	{
		auto add = [this, value, index, config]() { this->GetContext().Write(value, index, config); };
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	// ------- implement ILinkBind ---------

	virtual void SetLinkRouter(opendnp3::ILinkRouter& router) override final
	{
		stack.link.SetRouter(router);
	}

	virtual opendnp3::ILinkSession& GetLinkContext() override final
	{
		return stack.link;
	}

	// ------- implement ICommandProcessor ---------
	
	virtual void SelectAndOperate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->SelectAndOperateT(command, index, callback);
	}
	
	virtual void DirectOperate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->DirectOperateT(command, index, callback);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputInt16& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->SelectAndOperateT(command, index, callback);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputInt16& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->DirectOperateT(command, index, callback);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->SelectAndOperateT(command, index, callback);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->DirectOperateT(command, index, callback);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->SelectAndOperateT(command, index, callback);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->DirectOperateT(command, index, callback);
	}
	
	virtual void SelectAndOperate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->SelectAndOperateT(command, index, callback);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, opendnp3::ICommandCallback& callback) override final
	{
		this->DirectOperateT(command, index, callback);
	}

protected:

	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, opendnp3::ICommandCallback& callback)
	{
		auto context = &this->GetContext();
		auto action = [context, command, index, &callback]()
		{
			context->SelectAndOperate(command, index, callback);
		};
		context->pExecutor->PostLambda(action);
	}

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, opendnp3::ICommandCallback& callback)
	{
		auto context = &this->GetContext();
		auto action = [context, command, index, &callback]()
		{
			context->DirectOperate(command, index, callback);
		};
		context->pExecutor->PostLambda(action);
	}

	virtual opendnp3::MContext& GetContext() = 0;		
	
	openpal::LogRoot root;	
	opendnp3::StackStatistics statistics;
	IStackLifecycle* pLifecycle;
	opendnp3::TransportStack stack;	
};

}

#endif

