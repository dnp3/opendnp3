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
#ifndef OPENDNP3_LINKLAYERCONFIG_H
#define OPENDNP3_LINKLAYERCONFIG_H

#include "opendnp3/link/LinkConfig.h"

namespace opendnp3
{

/**
    Includes additional parameters that should not be externally configurable
*/
struct LinkLayerConfig : public LinkConfig
{
    LinkLayerConfig(const LinkConfig& config, bool respondToAnySource)
        : LinkConfig(config), respondToAnySource(respondToAnySource)
    {
    }

    /**
     * If true, the the link-layer will respond to any source address
     * user data frames will be passed up to transport reassembly for these frames
     */
    bool respondToAnySource;
};

} // namespace opendnp3

#endif
