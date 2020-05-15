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

#include "CommandSetOps.h"

#include "app/parsing/APDUParser.h"
#include "master/CommandTaskResult.h"
#include "master/ICommandHeader.h"

#include <algorithm>

namespace opendnp3
{

template<class T> IINField CommandSetOps::ProcessAny(const PrefixHeader& header, const ICollection<Indexed<T>>& values)
{
    if (header.headerIndex >= commands->m_headers.size()) // more response headers than request headers
    {
        return IINBit::PARAM_ERROR;
    }

    if (mode == Mode::Select)
    {
        commands->m_headers[header.headerIndex]->ApplySelectResponse(header.GetQualifierCode(), values);
    }
    else
    {
        commands->m_headers[header.headerIndex]->ApplyOperateResponse(header.GetQualifierCode(), values);
    }

    return IINField::Empty();
}

CommandSetOps::CommandSetOps(Mode mode_, CommandSet& commands_) : mode(mode_), commands(&commands_) {}

bool CommandSetOps::Write(const CommandSet& set, HeaderWriter& writer, IndexQualifierMode mode)
{
    for (auto& header : set.m_headers)
    {
        if (!header->Write(writer, mode))
        {
            return false;
        }
    }

    return true;
}

void CommandSetOps::InvokeCallback(const CommandSet& set, TaskCompletion result, const CommandResultCallbackT& callback)
{
    CommandTaskResult impl(result, set.m_headers);
    callback(impl);
}

CommandSetOps::SelectResult CommandSetOps::ProcessSelectResponse(CommandSet& set,
                                                                 const ser4cpp::rseq_t& headers,
                                                                 Logger* logger)
{
    CommandSetOps handler(Mode::Select, set);
    const auto result = APDUParser::Parse(headers, handler, logger);
    if (result != ParseResult::OK)
    {
        return SelectResult::FAIL_PARSE;
    }

    auto selected = [](const std::shared_ptr<ICommandHeader>& header) -> bool { return header->AreAllSelected(); };
    return std::all_of(set.m_headers.begin(), set.m_headers.end(), selected) ? SelectResult::OK
                                                                             : SelectResult::FAIL_SELECT;
}

CommandSetOps::OperateResult CommandSetOps::ProcessOperateResponse(CommandSet& set,
                                                                   const ser4cpp::rseq_t& headers,
                                                                   Logger* logger)
{
    CommandSetOps handler(Mode::Operate, set);
    return (APDUParser::Parse(headers, handler, logger) == ParseResult::OK) ? OperateResult::OK
                                                                            : OperateResult::FAIL_PARSE;
}

bool CommandSetOps::IsAllowed(uint32_t /*headerCount*/, GroupVariation gv, QualifierCode qc)
{
    switch (qc)
    {
    case (QualifierCode::UINT8_CNT_UINT8_INDEX):
    case (QualifierCode::UINT16_CNT_UINT16_INDEX):
        break;
    default:
        return false;
    }

    switch (gv)
    {
    case (GroupVariation::Group12Var1): //	CROB
    case (GroupVariation::Group41Var1): //	4 kinds of AO
    case (GroupVariation::Group41Var2):
    case (GroupVariation::Group41Var3):
    case (GroupVariation::Group41Var4):
        return true;
    default:
        return false;
    }
}

IINField CommandSetOps::ProcessHeader(const PrefixHeader& header,
                                      const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
    return this->ProcessAny(header, values);
}

IINField CommandSetOps::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
    return this->ProcessAny(header, values);
}

IINField CommandSetOps::ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
    return this->ProcessAny(header, values);
}

IINField CommandSetOps::ProcessHeader(const PrefixHeader& header,
                                      const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
    return this->ProcessAny(header, values);
}

IINField CommandSetOps::ProcessHeader(const PrefixHeader& header,
                                      const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
    return this->ProcessAny(header, values);
}

} // namespace opendnp3
