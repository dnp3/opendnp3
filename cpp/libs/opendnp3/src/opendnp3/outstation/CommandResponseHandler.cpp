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

#include "CommandResponseHandler.h"

#include <openpal/serialization/Serialization.h>

#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group41.h"

namespace opendnp3
{

	CommandResponseHandler::CommandResponseHandler(openpal::Logger logger, uint8_t maxCommands_, ICommandAction* pCommandAction_, HeaderWriter* pWriter_) :
	APDUHandlerBase(logger),
	pCommandAction(pCommandAction_),
	numRequests(0),
	numSuccess(0),
	maxCommands(maxCommands_),
	pWriter(pWriter_)
{
	
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas)
{
	this->RespondToHeader<ControlRelayOutputBlock, openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group12Var1::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas)
{
	this->RespondToHeader<AnalogOutputInt16, openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group41Var2::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas)
{
	this->RespondToHeader<AnalogOutputInt32, openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group41Var1::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas)
{
	this->RespondToHeader<AnalogOutputFloat32, openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group41Var3::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas)
{
	this->RespondToHeader<AnalogOutputDouble64, openpal::UInt16>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group41Var4::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas)
{
	this->RespondToHeader<ControlRelayOutputBlock, openpal::UInt8>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group12Var1::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas)
{
	this->RespondToHeader<AnalogOutputInt16, openpal::UInt8>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group41Var2::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas)
{
	this->RespondToHeader<AnalogOutputInt32, openpal::UInt8>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group41Var1::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas)
{
	this->RespondToHeader<AnalogOutputFloat32, openpal::UInt8>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group41Var3::Inst(), meas);
}

void CommandResponseHandler::_OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas)
{
	this->RespondToHeader<AnalogOutputDouble64, openpal::UInt8>(QualifierCode::UINT8_CNT_UINT8_INDEX, Group41Var4::Inst(), meas);
}

}
