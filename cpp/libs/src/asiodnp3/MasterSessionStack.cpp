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

#include "asiodnp3/MasterSessionStack.h"

#include "asiopal/StrandExecutor.h"
#include "asiodnp3/Conversions.h"
#include "asiodnp3/SocketSession.h"

using namespace opendnp3;

namespace asiodnp3
{
	std::shared_ptr<MasterSessionStack> MasterSessionStack::Create(
		openpal::Logger logger,
		asiopal::StrandExecutor& executor,
		std::shared_ptr<opendnp3::ISOEHandler> SOEHandler,
		std::shared_ptr<opendnp3::IMasterApplication> application,
		std::shared_ptr<SocketSession> session,
		opendnp3::ILinkTx& linktx,
		const opendnp3::MasterStackConfig& config
		)
	{
		return std::shared_ptr<MasterSessionStack>(new MasterSessionStack(logger, executor, SOEHandler, application, session, linktx, config));
	}

	MasterSessionStack::MasterSessionStack(
		openpal::Logger logger,
		asiopal::StrandExecutor& executor,
		std::shared_ptr<opendnp3::ISOEHandler> SOEHandler,
		std::shared_ptr<opendnp3::IMasterApplication> application,
		std::shared_ptr<SocketSession> session,
		opendnp3::ILinkTx& linktx,		
		const opendnp3::MasterStackConfig& config
		) :
		m_executor(&executor),
		m_handler(SOEHandler),
		m_application(application),
		m_session(session),
		m_statistics(),
		m_stack(logger, executor, *application, config.master.maxRxFragSize, &m_statistics, config.link),
		m_context(executor, logger, m_stack.transport, *SOEHandler, *application, config.master, opendnp3::NullTaskLock::Instance())
	{
		m_stack.link.SetRouter(linktx);	
		m_stack.transport.SetAppLayer(m_context);
	}

	void MasterSessionStack::OnLowerLayerUp()
	{
		m_stack.link.OnLowerLayerUp();
	}

	void MasterSessionStack::OnLowerLayerDown()
	{
		m_stack.link.OnLowerLayerDown();
	}

	bool MasterSessionStack::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
	{
		return m_stack.link.OnFrame(header, userdata);
	}

	void MasterSessionStack::BeginShutdown()
	{
		auto session = m_session;
		auto shutdown = [session](){
			session->BeginShutdown();
		};

		m_executor->strand.post(shutdown);
	}

	opendnp3::StackStatistics MasterSessionStack::GetStackStatistics()
	{
		auto self(shared_from_this());
		auto get = [self ](){ return self->m_statistics; };
		return m_executor->ReturnFrom<StackStatistics>(get);
	}

	opendnp3::MasterScan MasterSessionStack::AddScan(openpal::TimeDuration period, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		auto self(shared_from_this());
		auto builder = ConvertToLambda(headers);
		auto get = [self, period, builder, config]() -> opendnp3::MasterScan { return self->m_context.AddScan(period, builder, config); };
		return m_executor->ReturnFrom<MasterScan>(get);
	}

	opendnp3::MasterScan MasterSessionStack::AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	opendnp3::MasterScan MasterSessionStack::AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	opendnp3::MasterScan MasterSessionStack::AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::Scan(const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::ScanAllObjects(opendnp3::GroupVariationID gvId, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::ScanClasses(const opendnp3::ClassField& field, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::Write(const opendnp3::TimeAndInterval& value, uint16_t index, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::Restart(opendnp3::RestartType op, const opendnp3::RestartOperationCallbackT& callback, opendnp3::TaskConfig config)
	{
		throw std::exception();
	}

	void MasterSessionStack::PerformFunction(const std::string& name, opendnp3::FunctionCode func, const std::vector<opendnp3::Header>& headers, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	/// --- ICommandProcessor ---

	void MasterSessionStack::SelectAndOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}

	void MasterSessionStack::DirectOperate(opendnp3::CommandSet&& commands, const opendnp3::CommandCallbackT& callback, const opendnp3::TaskConfig& config)
	{
		throw std::exception();
	}
}



