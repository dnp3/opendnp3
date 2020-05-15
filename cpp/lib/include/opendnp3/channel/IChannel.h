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
#ifndef OPENDNP3_ICHANNEL_H
#define OPENDNP3_ICHANNEL_H

#include "opendnp3/IResource.h"
#include "opendnp3/gen/ChannelState.h"
#include "opendnp3/link/LinkStatistics.h"
#include "opendnp3/logging/LogLevels.h"
#include "opendnp3/master/IMaster.h"
#include "opendnp3/master/IMasterApplication.h"
#include "opendnp3/master/ISOEHandler.h"
#include "opendnp3/master/MasterStackConfig.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstation.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/OutstationStackConfig.h"

#include <memory>

namespace opendnp3
{

/**
 * Represents a communication channel upon which masters and outstations can be bound.
 */
class IChannel : public IResource
{
public:
    virtual ~IChannel() {}

    /**
     * Synchronously read the channel statistics
     */
    virtual LinkStatistics GetStatistics() = 0;

    /**
     *  @return The current logger settings for this channel
     */
    virtual opendnp3::LogLevels GetLogFilters() const = 0;

    /**
     *  @param filters Adjust the filters to this value
     */
    virtual void SetLogFilters(const opendnp3::LogLevels& filters) = 0;

    /**
     * Add a master to the channel
     *
     * @param id An ID that gets used for logging
     * @param SOEHandler Callback object for all received measurements
     * @param application The master application bound to the master session
     * @param config Configuration object that controls how the master behaves
     *
     * @return shared_ptr to the running master
     */
    virtual std::shared_ptr<IMaster> AddMaster(const std::string& id,
                                               std::shared_ptr<ISOEHandler> SOEHandler,
                                               std::shared_ptr<IMasterApplication> application,
                                               const MasterStackConfig& config)
        = 0;

    /**
     * Add an outstation to the channel
     *
     * @param id An ID that gets used for logging
     * @param commandHandler Callback object for handling command requests
     * @param application Callback object for user code
     * @param config Configuration object that controls how the outstation behaves
     * @return shared_ptr to the running outstation
     */
    virtual std::shared_ptr<IOutstation> AddOutstation(const std::string& id,
                                                       std::shared_ptr<ICommandHandler> commandHandler,
                                                       std::shared_ptr<IOutstationApplication> application,
                                                       const OutstationStackConfig& config)
        = 0;
};

} // namespace opendnp3

#endif
