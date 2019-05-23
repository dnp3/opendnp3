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
#ifndef OPENDNP3_OUTSTATIONSTACKCONFIG_H
#define OPENDNP3_OUTSTATIONSTACKCONFIG_H

#include "opendnp3/link/LinkConfig.h"
#include "opendnp3/outstation/DatabaseSizes.h"
#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/outstation/OutstationConfig.h"

#include "asiodnp3/DatabaseConfig.h"

namespace asiodnp3
{

/**
    A composite configuration struct that contains all the config
    information for a dnp3 outstation stack
*/
struct OutstationStackConfig
{

    OutstationStackConfig(const opendnp3::DatabaseSizes& dbSizes) : dbConfig(dbSizes), link(false, false) {}

    OutstationStackConfig() = delete;

    // Configuration of the database
    DatabaseConfig dbConfig;

    /// Outstation config
    opendnp3::OutstationConfig outstation;

    /// Link layer config
    opendnp3::LinkConfig link;
};

} // namespace asiodnp3

#endif
