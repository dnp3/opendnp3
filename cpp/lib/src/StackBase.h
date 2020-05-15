/*
 * Copyright 2013-2020 Automatak, LLC
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
#ifndef OPENDNP3_STACKBASE_H
#define OPENDNP3_STACKBASE_H

#include "IResourceManager.h"
#include "channel/IOHandler.h"
#include "transport/TransportStack.h"

#include "opendnp3/IStack.h"

#include <exe4cpp/asio/StrandExecutor.h>

namespace opendnp3
{

/**
 * Base class for masters or outstations
 */
class StackBase
{

protected:
    StackBase(const Logger& logger,
              const std::shared_ptr<exe4cpp::StrandExecutor>& executor,
              const std::shared_ptr<ILinkListener>& listener,
              const std::shared_ptr<IOHandler>& iohandler,
              const std::shared_ptr<IResourceManager>& manager,
              uint32_t maxRxFragSize,
              const LinkLayerConfig& config)
        : logger(logger),
          executor(executor),
          iohandler(iohandler),
          manager(manager),
          tstack(logger, executor, listener, maxRxFragSize, config)
    {
    }

    StackStatistics CreateStatistics() const
    {
        return StackStatistics(tstack.link->GetStatistics(), tstack.transport->GetStatistics());
    }

    template<class T> void PerformShutdown(const std::shared_ptr<T>& self);

    Logger logger;
    const std::shared_ptr<exe4cpp::StrandExecutor> executor;
    const std::shared_ptr<IOHandler> iohandler;
    const std::shared_ptr<IResourceManager> manager;
    TransportStack tstack;
};

template<class T> void StackBase::PerformShutdown(const std::shared_ptr<T>& self)
{
    auto shutdown = [self] {
        self->iohandler->Remove(self);

        // since posting to a strand from the strand is ordered, posting
        // this forces the MasterStack to hang around long enough for
        // any previously submitted post operations on the strand to complete
        auto detach = [self]() { self->manager->Detach(self); };
        self->executor->post(detach);
    };

    this->executor->block_until_and_flush(shutdown);
}

} // namespace opendnp3

#endif
