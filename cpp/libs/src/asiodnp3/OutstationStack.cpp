/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "OutstationStack.h"

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

template<class T, class U> void assign(const T& config, U& view)
{
    for (auto i = 0; i < view.Size(); ++i)
    {
        view[i].config = config[i];
    }
}

OutstationStack::OutstationStack(const Logger& logger,
                                 const std::shared_ptr<Executor>& executor,
                                 const std::shared_ptr<ICommandHandler>& commandHandler,
                                 const std::shared_ptr<IOutstationApplication>& application,
                                 const std::shared_ptr<IOHandler>& iohandler,
                                 const std::shared_ptr<IResourceManager>& manager,
                                 const OutstationStackConfig& config)
    :

      StackBase(logger,
                executor,
                application,
                iohandler,
                manager,
                config.outstation.params.maxRxFragSize,
                LinkLayerConfig(config.link, config.outstation.params.respondToAnyMaster)),
      ocontext(Addresses(config.link.LocalAddr, config.link.RemoteAddr),
               config.outstation,
               config.dbConfig.sizes,
               logger,
               executor,
               tstack.transport,
               commandHandler,
               application)
{
    this->tstack.transport->SetAppLayer(ocontext);

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
    assign(config.dbConfig.octetString, view.octetStrings);
}

bool OutstationStack::Enable()
{
    auto action = [self = shared_from_this()] { return self->iohandler->Enable(self); };
    return this->executor->ReturnFrom<bool>(action);
}

bool OutstationStack::Disable()
{
    auto action = [self = shared_from_this()] { return self->iohandler->Disable(self); };
    return this->executor->ReturnFrom<bool>(action);
}

void OutstationStack::Shutdown()
{
    this->PerformShutdown(shared_from_this());
}

StackStatistics OutstationStack::GetStackStatistics()
{
    auto get = [self = shared_from_this()] { return self->CreateStatistics(); };
    return this->executor->ReturnFrom<StackStatistics>(get);
}

void OutstationStack::SetLogFilters(const LogFilters& filters)
{
    auto set = [self = this->shared_from_this(), filters]() { self->logger.SetFilters(filters); };
    this->executor->strand.post(set);
}

void OutstationStack::SetRestartIIN()
{
    // this doesn't need to be synchronous, just post it
    auto set = [self = this->shared_from_this()]() { self->ocontext.SetRestartIIN(); };
    this->executor->strand.post(set);
}

void OutstationStack::Apply(const Updates& updates)
{
    if (updates.IsEmpty())
        return;

    auto task = [self = this->shared_from_this(), updates]() {
        updates.Apply(self->ocontext.GetUpdateHandler());
        self->ocontext.CheckForTaskStart(); // force the outstation to check for updates
    };

    this->executor->strand.post(task);
}

} // namespace asiodnp3
