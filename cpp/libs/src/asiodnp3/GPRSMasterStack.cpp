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

#include "asiodnp3/GPRSMasterStack.h"

#include "asiopal/StrandExecutor.h"

using namespace opendnp3;

namespace asiodnp3
{

	GPRSMasterStack::GPRSMasterStack(
		openpal::LogRoot& root,		
		asiopal::StrandExecutor& executor,
		opendnp3::ILinkTx& linktx,
		opendnp3::ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		const opendnp3::MasterStackConfig& config
		) :
		m_executor(&executor),
		m_statistics(),
		m_stack(root, executor, application, config.master.maxRxFragSize, &m_statistics, config.link),
		m_context(executor, root, m_stack.transport, SOEHandler, application, config.master, opendnp3::NullTaskLock::Instance())
	{
		m_stack.link.SetRouter(linktx);	
		m_stack.transport.SetAppLayer(m_context);
	}

	void GPRSMasterStack::OnLowerLayerUp()
	{
		m_stack.link.OnLowerLayerUp();
	}

	void GPRSMasterStack::OnLowerLayerDown()
	{
		m_stack.link.OnLowerLayerDown();
	}

	bool GPRSMasterStack::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
	{
		return m_stack.link.OnFrame(header, userdata);
	}

	void GPRSMasterStack::BeginShutdown()
	{
		throw std::exception("not implemented");
	}

	opendnp3::StackStatistics GPRSMasterStack::GetStackStatistics()
	{
		throw std::exception("not implemented");
	}

	opendnp3::MasterScan GPRSMasterStack::AddScan(openpal::TimeDuration period, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	opendnp3::MasterScan GPRSMasterStack::AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	opendnp3::MasterScan GPRSMasterStack::AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	opendnp3::MasterScan GPRSMasterStack::AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::Scan(const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::ScanAllObjects(opendnp3::GroupVariationID gvId, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::ScanClasses(const opendnp3::ClassField& field, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::Write(const opendnp3::TimeAndInterval& value, uint16_t index, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::Restart(opendnp3::RestartType op, const opendnp3::RestartOperationCallbackT& callback, opendnp3::TaskConfig config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::PerformFunction(const std::string& name, opendnp3::FunctionCode func, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	/// --- ICommandProcessor ---

	void GPRSMasterStack::SelectAndOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}

	void GPRSMasterStack::DirectOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		throw std::exception("not implemented");
	}
}



