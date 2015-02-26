/**
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
#ifndef SECAUTHV5_AGGRESSIVEMODEPARSER_H
#define SECAUTHV5_AGGRESSIVEMODEPARSER_H

#include <openpal/util/Uncopyable.h>
#include <openpal/util/Comparisons.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/app/parsing/ParseResult.h>
#include <opendnp3/objects/Group120Var5.h>

#include <cstdint>
#include <utility>

#include <opendnp3/objects/Group120.h>
#include <opendnp3/objects/Group120Var9.h>

namespace secauthv5
{

struct AggModeObjects
{
	opendnp3::Group120Var3 request;
	openpal::ReadBufferView objects;
	opendnp3::Group120Var9 hmac;
};


struct AggressiveModeParser : openpal::PureStatic
{	
	static std::pair<opendnp3::ParseResult, bool> IsAggressiveMode(openpal::ReadBufferView objects, opendnp3::Group120Var3& request, openpal::Logger* pLogger);
	


};

}

#endif

