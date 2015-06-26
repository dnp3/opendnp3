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

#include "MasterCommandProcessor.h"

#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group41.h"

namespace opendnp3
{
	MasterCommandProcessor::MasterCommandProcessor(MContext& state) : pState(&state)
	{}

	void MasterCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
	{
		this->SelectAndOperateT(command, index, callback, Group12Var1::Inst());
	}

	void MasterCommandProcessor::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
	{
		this->DirectOperateT(command, index, callback, Group12Var1::Inst());
	}

	void MasterCommandProcessor::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
	{
		this->SelectAndOperateT(command, index, callback, Group41Var2::Inst());
	}

	void MasterCommandProcessor::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
	{
		this->DirectOperateT(command, index, callback, Group41Var2::Inst());
	}

	void MasterCommandProcessor::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
	{
		this->SelectAndOperateT(command, index, callback, Group41Var1::Inst());
	}

	void MasterCommandProcessor::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
	{
		this->DirectOperateT(command, index, callback, Group41Var1::Inst());
	}

	void MasterCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
	{
		this->SelectAndOperateT(command, index, callback, Group41Var3::Inst());
	}

	void MasterCommandProcessor::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
	{
		this->DirectOperateT(command, index, callback, Group41Var3::Inst());
	}

	void MasterCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
	{
		this->SelectAndOperateT(command, index, callback, Group41Var4::Inst());
	}

	void MasterCommandProcessor::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
	{
		this->DirectOperateT(command, index, callback, Group41Var4::Inst());
	}
}

