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
#include <opendnp3/master/MasterContext.h>
#include <opendnp3/transport/TransportStack.h>

#include "asiopal/StrandExecutor.h"

#include "asiodnp3/IMaster.h"

#include "Conversions.h"
#include "asiodnp3/MasterStackConfig.h"

#include <assert.h>

namespace asiodnp3
{

template <class Interface>
class MasterStackBase : public Interface
{
public:

	MasterStackBase(
	    const openpal::Logger& logger,
		const std::shared_ptr<asiopal::StrandExecutor>& executor,
	    opendnp3::ILinkListener& listener,
	    const MasterStackConfig& config	    
	) :
		logger(logger),		
		executor(executor),
		stack(logger, *executor.get(), listener, config.master.maxRxFragSize, &statistics, config.link),		
		pContext(nullptr)
	{

	}

	virtual ~MasterStackBase() {}

	virtual bool Enable() override final
	{
		//return pLifecycle->EnableRoute(&stack.link);
		// TODO
		return false;
	}

	virtual bool Disable() override final
	{
		//return pLifecycle->DisableRoute(&stack.link);
		// TODO
		return false;
	}

	virtual void Shutdown() override final
	{
		//return pLifecycle->Shutdown(&stack.link, this);
		// TODO
	}

	virtual void SetLogFilters(const openpal::LogFilters& filters) override final
	{
		/*
		auto set = [this, filters]()
		{
			this->logger.SetFilters(filters);
		};

		pLifecycle->GetExecutor().BlockFor(set);
		*/
		// TODO
	}

	virtual opendnp3::StackStatistics GetStackStatistics() override final
	{
		auto get = [this]()
		{
			return this->statistics;
		};
		return this->executor->ReturnFrom<opendnp3::StackStatistics>(get);
	}

	// ------- Periodic scan API ---------

	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		auto builder = ConvertToLambda(headers);
		auto add = [this, builder, period, config]()
		{
			return this->pContext->AddScan(period, builder, config);
		};
		return this->executor->ReturnFrom<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, period, config]()
		{
			return this->pContext->AddAllObjectsScan(gvId, period, config);
		};
		return this->executor->ReturnFrom<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, field, period, config]()
		{
			return this->pContext->AddClassScan(field, period, config);
		};
		return this->executor->ReturnFrom<opendnp3::MasterScan>(add);
	}

	virtual opendnp3::MasterScan AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const opendnp3::TaskConfig& config) override final
	{
		auto add = [this, gvId, start, stop, period, config]()
		{
			return this->pContext->AddRangeScan(gvId, start, stop, period, config);
		};
		return this->executor->ReturnFrom<opendnp3::MasterScan>(add);
	}

	// ------- Adhoc scan API ---------

	virtual void Scan(const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		/*
		auto builder = ConvertToLambda(headers);
		auto add = [this, builder, config]()
		{
			return this->pContext->Scan(builder, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	virtual void ScanAllObjects(opendnp3::GroupVariationID gvId, const opendnp3::TaskConfig& config) override final
	{
		/*
		auto add = [this, gvId, config]()
		{
			this->pContext->ScanAllObjects(gvId, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	virtual void ScanClasses(const opendnp3::ClassField& field, const opendnp3::TaskConfig& config) override final
	{
		/*
		auto add = [this, field, config]()
		{
			this->pContext->ScanClasses(field, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	virtual void ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const opendnp3::TaskConfig& config) override final
	{
		/*
		auto add = [this, gvId, start, stop, config]()
		{
			this->pContext->ScanRange(gvId, start, stop, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	// ------- Other adhoc methods -------

	virtual void Write(const opendnp3::TimeAndInterval& value, uint16_t index, const opendnp3::TaskConfig& config)  override final
	{
		/*
		auto add = [this, value, index, config]()
		{
			this->pContext->Write(value, index, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	virtual void Restart(opendnp3::RestartType op, const opendnp3::RestartOperationCallbackT& callback, opendnp3::TaskConfig config) override final
	{
		/*
		auto add = [this, op, callback, config]()
		{
			this->pContext->Restart(op, callback, config);
		};
		return pLifecycle->GetExecutor().BlockFor(add);
		*/
		// TODO
	}

	virtual void PerformFunction(const std::string& name, opendnp3::FunctionCode fc, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config) override final
	{
		/*
		auto builder = ConvertToLambda(headers);
		auto add = [this, name, fc, builder, config]()
		{
			this->pContext->PerformFunction(name, fc, builder, config);
		};
		return this->executor->ReturnFrom<void>(add);
		*/
		// TODO
	}

	// ------- implement ILinkBind ---------

	void SetLinkRouter(opendnp3::ILinkTx& router)
	{
		stack.link.SetRouter(router);
	}

	opendnp3::ILinkSession& GetLinkContext()
	{
		return stack.link;
	}

	// ------- implement ICommandProcessor ---------

	virtual void SelectAndOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		/// this is to work around the fact that c++11 doesn't have generic move capture
		auto set = new opendnp3::CommandSet(std::move(commands));

		auto action = [this, set, config, callback]()
		{
			// delete pointer at the end of the lambda
			std::unique_ptr<opendnp3::CommandSet> deleted(set);
			this->pContext->SelectAndOperate(std::move(*set), callback, config);
		};

		this->executor->PostToStrand(action);
	}

	virtual void DirectOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config) override final
	{
		auto set = new opendnp3::CommandSet(std::move(commands));

		auto action = [this, set, config, callback]()
		{
			std::unique_ptr<opendnp3::CommandSet> deleted(set);
			this->pContext->DirectOperate(std::move(*set), callback, config);
		};

		this->executor->PostToStrand(action);
	}

protected:

	void SetContext(opendnp3::MContext& context)
	{
		assert(pContext == nullptr);
		this->pContext = &context;
		this->stack.transport.SetAppLayer(context);
	}

	
	openpal::Logger logger;
	const std::shared_ptr<asiopal::StrandExecutor> executor;
	opendnp3::StackStatistics statistics;	
	opendnp3::TransportStack stack;
	

private:

	opendnp3::MContext* pContext;
};

}

#endif
