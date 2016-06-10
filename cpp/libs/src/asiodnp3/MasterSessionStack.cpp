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
#include "asiodnp3/LinkSession.h"

using namespace opendnp3;

namespace asiodnp3
{
std::shared_ptr<MasterSessionStack> MasterSessionStack::Create(
    openpal::Logger logger,
    std::shared_ptr<asiopal::StrandExecutor> executor,
    std::shared_ptr<ISOEHandler> SOEHandler,
    std::shared_ptr<IMasterApplication> application,
    std::shared_ptr<LinkSession> session,
    ILinkTx& linktx,
    const MasterStackConfig& config
)
{
	return std::shared_ptr<MasterSessionStack>(new MasterSessionStack(logger, executor, SOEHandler, application, session, linktx, config));
}

MasterSessionStack::MasterSessionStack(
    openpal::Logger logger,
    std::shared_ptr<asiopal::StrandExecutor> executor,
    std::shared_ptr<ISOEHandler> SOEHandler,
    std::shared_ptr<IMasterApplication> application,
    std::shared_ptr<LinkSession> session,
    ILinkTx& linktx,
    const MasterStackConfig& config
) :
	m_executor(executor),
	m_handler(SOEHandler),
	m_application(application),
	m_session(session),
	m_statistics(),
	m_stack(logger, *m_executor, *application, config.master.maxRxFragSize, &m_statistics, config.link),
	m_context(*m_executor, logger, m_stack.transport, *SOEHandler, *application, config.master, NullTaskLock::Instance())
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

	// now we can release the socket session
	m_session.reset();
}

bool MasterSessionStack::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
{
	return m_stack.link.OnFrame(header, userdata);
}

void MasterSessionStack::SetLogFilters(const openpal::LogFilters& filters)
{
	auto set = [this, filters]()
	{
		this->m_session->SetLogFilters(filters);
	};

	this->m_executor->m_strand.post(set);
}

void MasterSessionStack::BeginShutdown()
{
	auto session = m_session;
	auto shutdown = [session]()
	{
		session->BeginShutdown();
	};

	m_executor->m_strand.post(shutdown);
}

StackStatistics MasterSessionStack::GetStackStatistics()
{
	auto self(shared_from_this());
	auto get = [self ]()
	{
		return self->m_statistics;
	};
	return m_executor->ReturnFrom<StackStatistics>(get);
}

MasterScan MasterSessionStack::AddScan(openpal::TimeDuration period, const std::vector<Header>& headers, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto builder = ConvertToLambda(headers);
	auto get = [self, period, builder, config]() -> MasterScan { return self->m_context.AddScan(period, builder, config); };
	return m_executor->ReturnFrom<MasterScan>(get);
}

MasterScan MasterSessionStack::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto get = [self, gvId, period, config] { return self->m_context.AddAllObjectsScan(gvId, period, config); };
	return m_executor->ReturnFrom<MasterScan>(get);
}

MasterScan MasterSessionStack::AddClassScan(const ClassField& field, openpal::TimeDuration period, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto get = [self, field, period, config] { return self->m_context.AddClassScan(field, period, config); };
	return m_executor->ReturnFrom<MasterScan>(get);
}

MasterScan MasterSessionStack::AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto get = [self, gvId, start, stop, period, config] { return self->m_context.AddRangeScan(gvId, start, stop, period, config); };
	return m_executor->ReturnFrom<MasterScan>(get);
}

void MasterSessionStack::Scan(const std::vector<Header>& headers, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto builder = ConvertToLambda(headers);
	auto action = [self, builder, config]() -> void { self->m_context.Scan(builder, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::ScanAllObjects(GroupVariationID gvId, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto action = [self, gvId, config]() -> void { self->m_context.ScanAllObjects(gvId, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::ScanClasses(const ClassField& field, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto action = [self, field, config]() -> void { self->m_context.ScanClasses(field, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto action = [self, gvId, start, stop, config]() -> void { self->m_context.ScanRange(gvId, start, stop, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto action = [self, value, index, config]() -> void { self->m_context.Write(value, index, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::Restart(RestartType op, const RestartOperationCallbackT& callback, TaskConfig config)
{
	auto self(shared_from_this());
	auto action = [self, op, callback, config]() -> void { self->m_context.Restart(op, callback, config); };
	return m_executor->m_strand.post(action);
}

void MasterSessionStack::PerformFunction(const std::string& name, FunctionCode func, const std::vector<Header>& headers, const TaskConfig& config)
{
	auto self(shared_from_this());
	auto builder = ConvertToLambda(headers);
	auto action = [self, name, func, builder, config]() -> void { self->m_context.PerformFunction(name, func, builder, config); };
	return m_executor->m_strand.post(action);
}

/// --- ICommandProcessor ---

void MasterSessionStack::SelectAndOperate(CommandSet&& commands, const CommandCallbackT& callback, const TaskConfig& config)
{
	auto self(shared_from_this());

	// this is required b/c move capture not supported in C++11
	auto set = std::make_shared<CommandSet>(std::move(commands));

	auto action = [self, set, config, callback]() -> void
	{
		self->m_context.SelectAndOperate(std::move(*set), callback, config);
	};
	m_executor->m_strand.post(action);
}

void MasterSessionStack::DirectOperate(CommandSet&& commands, const CommandCallbackT& callback, const TaskConfig& config)
{
	auto self(shared_from_this());

	// this is required b/c move capture not supported in C++11
	auto set = std::make_shared<CommandSet>(std::move(commands));

	auto action = [self, set, config, callback]() -> void
	{
		self->m_context.DirectOperate(std::move(*set), callback, config);
	};
	m_executor->m_strand.post(action);
}
}



