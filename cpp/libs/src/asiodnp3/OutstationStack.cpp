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
#include "OutstationStack.h"

#include <asiopal/ASIOExecutor.h>

using namespace opendnp3;

namespace asiodnp3
{

template <class T, class U>
void assign(const T& config, U& view)
{
	for (auto i = 0; i < view.Size(); ++i)
	{
		view[i].config = config[i];
	}
}

OutstationStack::OutstationStack(
    std::unique_ptr<openpal::LogRoot> root,
    openpal::IExecutor& executor,
    std::shared_ptr<opendnp3::ICommandHandler> commandHandler,
    std::shared_ptr<opendnp3::IOutstationApplication> application,
    const OutstationStackConfig& config,
    IStackLifecycle& lifecycle) :

	OutstationStackBase(std::move(root), executor, *application, config, lifecycle),
	commandHandler(commandHandler),
	application(application),
	ocontext(config.outstation, config.dbConfig.sizes, this->root->logger, executor, stack.transport, *commandHandler, *application)
{
	this->SetContext(ocontext);

	// apply the database configuration
	auto view = ocontext.GetConfigView();

	assign(config.dbConfig.binary, view.binaries);
	assign(config.dbConfig.doubleBinary, view.doubleBinaries);
	assign(config.dbConfig.analog, view.analogs);
	assign(config.dbConfig.counter, view.counters);
	assign(config.dbConfig.frozenCounter, view.frozenCounters);
	assign(config.dbConfig.boStatus, view.binaryOutputStatii);
	assign(config.dbConfig.aoStatus, view.analogOutputStatii);
	assign(config.dbConfig.timeAndInterval, view.timeAndIntervals);
	
}

}

