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
#include "LinkRoute.h"

#include <sstream>

namespace opendnp3
{

LinkRoute::LinkRoute(const uint16_t aRemoteAddr, const uint16_t aLocalAddr) :
	remote(aRemoteAddr),
	local(aLocalAddr)
{}

LinkRoute::LinkRoute() : remote(0), local(0) {}

std::string LinkRoute::ToString() const
{
	std::ostringstream oss;
	oss << " Local: " << this->local << " Remote: " << this->remote;
	return oss.str();
}

}


