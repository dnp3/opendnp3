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
#ifndef OPENDNP3_COMMAND_SET_H
#define OPENDNP3_COMMAND_SET_H

#include "opendnp3/master/ICommandCollection.h"

#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"

#include <vector>
#include <memory>

namespace opendnp3
{

class ICommandHeader;

class CommandSet
{	

	friend class CommandSetOps;

public:	

	CommandSet() {}	
	CommandSet(CommandSet&& other);
		
	ICommandCollection<ControlRelayOutputBlock>& StartHeaderCROB();	
	ICommandCollection<AnalogOutputInt32>& StartHeaderAOInt32();	
	ICommandCollection<AnalogOutputInt16>& StartHeaderAOInt16();	
	ICommandCollection<AnalogOutputFloat32>& StartHeaderAOFloat32();	
	ICommandCollection<AnalogOutputDouble64>& StartHeaderAODouble64();	
	
private:
	
	CommandSet(const CommandSet&) = delete;
	CommandSet& operator= (const CommandSet& other) = delete;
	
	std::vector<std::unique_ptr<ICommandHeader>> m_headers;	
};

}

#endif
