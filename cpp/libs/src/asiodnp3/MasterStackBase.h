/*
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
#include <opendnp3/master/MasterContext.h>
#include <opendnp3/transport/TransportStack.h>

#include <asiopal/ASIOExecutor.h>

#include "asiodnp3/IStackLifecycle.h"
#include "asiodnp3/IMaster.h"
#include "asiodnp3/ILinkBind.h"

#include "Conversions.h"

#include <assert.h>

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
	    opendnp3::ILinkListener& listener,
	    const opendnp3::MasterStackConfig& config,
	    IStackLifecycle& lifecycle
	) :
		root(root_, id),
		pLifecycle(&lifecycle),
		stack(root, executor, listener, config.master.maxRxFragSize, &statistics, config.link),
		pASIOExecutor(&executor),
		pContext(nullptr)
	{

	}

	virtual ~MasterStackBase() {}

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
		auto get = [this]()
		{
			return this->statistics;
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::StackStatistics>(get);
	}

	// ------- Periodic scan API ---------

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto builder = ConvertToLambda(headers);
		auto add = [this, builder, period, config]()
		{
			return this->pContext->AddScan(period, builder, config);
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, period, config]()
		{
			return this->pContext->AddAllObjectsScan(gvId, period, config);
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, field, period, config]()
		{
			return this->pContext->AddClassScan(field, period, config);
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, start, stop, period, config]()
		{
			return this->pContext->AddRangeScan(gvId, start, stop, period, config);
		};
		return pLifecycle->GetExecutor().ReturnBlockFor<opendnp3::MasterScan>(add);
	}

	// ------- Adhoc scan API ---------

	virtual void Scan(const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto builder = ConvertToLambda(headers);
		auto add = [this, builder, config]()
		{
			return this->pContext->Scan(builder, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanAllObjects(opendnp3::GroupVariationID gvId, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, config]()
		{
			this->pContext->ScanAllObjects(gvId, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanClasses(const opendnp3::ClassField& field, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, field, config]()
		{
			this->pContext->ScanClasses(field, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, start, stop, config]()
		{
			this->pContext->ScanRange(gvId, start, stop, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	// ------- Other adhoc methods -------

	virtual void Write(const opendnp3::TimeAndInterval& value, uint16_t index, const opendnp3::TaskConfig& config)  override final
	{
		auto add = [this, value, index, config]()
		{
			this->pContext->Write(value, index, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
	}

	virtual void PerformFunction(const std::string& name, opendnp3::FunctionCode fc, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto builder = ConvertToLambda(headers);
		auto add = [this, name, fc, builder, config]()
		{
			this->pContext->PerformFunction(name, fc, builder, config);
		};
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

	virtual void SelectAndOperate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->SelectAndOperateT(command, index, callback, config);
	}

	virtual void DirectOperate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->DirectOperateT(command, index, callback, config);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputInt16& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->SelectAndOperateT(command, index, callback, config);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputInt16& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->DirectOperateT(command, index, callback, config);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->SelectAndOperateT(command, index, callback, config);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputInt32& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->DirectOperateT(command, index, callback, config);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->SelectAndOperateT(command, index, callback, config);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->DirectOperateT(command, index, callback, config);
	}

	virtual void SelectAndOperate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->SelectAndOperateT(command, index, callback, config);
	}

	virtual void DirectOperate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		this->DirectOperateT(command, index, callback, config);
	}

protected:

	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		auto action = [this, command, index, config, callback]()
		{
			this->pContext->SelectAndOperate(command, index, callback, config);
		};
		this->pASIOExecutor->strand.post(action);
	}

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		auto action = [this, command, index, config, callback]()
		{
			this->pContext->DirectOperate(command, index, callback, config);
		};
		this->pASIOExecutor->strand.post(action);
	}

	void SetContext(opendnp3::MContext& context)
	{
		assert(pContext == nullptr);
		this->pContext = &context;
		this->stack.transport.SetAppLayer(&context);
	}

	openpal::LogRoot root;
	opendnp3::StackStatistics statistics;
	IStackLifecycle* pLifecycle;
	opendnp3::TransportStack stack;
	asiopal::ASIOExecutor* pASIOExecutor;

private:

	opendnp3::MContext* pContext;
};

}

#endif

