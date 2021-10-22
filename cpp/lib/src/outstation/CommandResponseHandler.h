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
#ifndef OPENDNP3_COMMANDRESPONSEHANDLER_H
#define OPENDNP3_COMMANDRESPONSEHANDLER_H

#include "app/APDUResponse.h"
#include "app/parsing/IAPDUHandler.h"
#include "outstation/ICommandAction.h"

#include "opendnp3/logging/Logger.h"

namespace opendnp3
{

class CommandResponseHandler : public IAPDUHandler
{
public:
    CommandResponseHandler(uint32_t maxCommands_, ICommandAction* pCommandAction_, HeaderWriter* pWriter_);

    bool AllCommandsSuccessful() const
    {
        return numRequests == numSuccess;
    }

    virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override final;

private:
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<ControlRelayOutputBlock>>& meas) override final;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt16>>& meas) override final;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputInt32>>& meas) override final;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputFloat32>>& meas) override final;
    virtual IINField ProcessHeader(const PrefixHeader& header,
                                   const ICollection<Indexed<AnalogOutputDouble64>>& meas) override final;

    IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record,
                                       const ICollection<Indexed<ControlRelayOutputBlock>>& meas);
    IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas);
    IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas);
    IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record,
                                       const ICollection<Indexed<AnalogOutputFloat32>>& meas);
    IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record,
                                       const ICollection<Indexed<AnalogOutputDouble64>>& meas);

    IINField ProcessIndexPrefixOneByte(const HeaderRecord& record,
                                       const ICollection<Indexed<ControlRelayOutputBlock>>& meas);
    IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas);
    IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas);
    IINField ProcessIndexPrefixOneByte(const HeaderRecord& record,
                                       const ICollection<Indexed<AnalogOutputFloat32>>& meas);
    IINField ProcessIndexPrefixOneByte(const HeaderRecord& record,
                                       const ICollection<Indexed<AnalogOutputDouble64>>& meas);

    ICommandAction* pCommandAction;
    uint32_t numRequests;
    uint32_t numSuccess;
    const uint32_t maxCommands;
    HeaderWriter* pWriter;

    template<class T> IINField ProcessAny(const HeaderRecord& record, const ICollection<Indexed<T>>& meas);

    template<class Target, class IndexType>
    IINField RespondToHeader(QualifierCode qualifier,
                             const DNP3Serializer<Target>& serializer,
                             const ICollection<Indexed<Target>>& values);

    template<class Target, class IndexType>
    IINField RespondToHeaderWithIterator(QualifierCode qualifier,
                                         const DNP3Serializer<Target>& serializer,
                                         const ICollection<Indexed<Target>>& values,
                                         PrefixedWriteIterator<IndexType, Target>* pIterator = nullptr);

    template<class Target> CommandStatus ProcessCommand(const Target& command, uint16_t index);
};

template<class T>
IINField CommandResponseHandler::ProcessAny(const HeaderRecord& record, const ICollection<Indexed<T>>& meas)
{
    if (record.GetQualifierCode() == QualifierCode::UINT8_CNT_UINT8_INDEX)
    {
        return ProcessIndexPrefixOneByte(record, meas);
    }
    else
    {
        return ProcessIndexPrefixTwoByte(record, meas);
    }
}

template<class Target, class IndexType>
IINField CommandResponseHandler::RespondToHeaderWithIterator(QualifierCode qualifier,
                                                             const DNP3Serializer<Target>& serializer,
                                                             const ICollection<Indexed<Target>>& values,
                                                             PrefixedWriteIterator<IndexType, Target>* pIterator)
{
    IINField ret;

    auto process = [this, pIterator, &ret](const Indexed<Target>& pair) {
        Target response(pair.value);
        response.status = this->ProcessCommand(pair.value, pair.index);

        switch (response.status)
        {
        case (CommandStatus::SUCCESS):
            ++this->numSuccess;
            break;
        case (CommandStatus::NOT_SUPPORTED):
            ret.SetBit(IINBit::PARAM_ERROR);
            break;
        default:
            break;
        }

        if (pIterator)
        {
            pIterator->Write(response, static_cast<typename IndexType::type_t>(pair.index));
        }
    };

    values.ForeachItem(process);

    return ret;
}

template<class Target> CommandStatus CommandResponseHandler::ProcessCommand(const Target& command, uint16_t index)
{
    if (numRequests < maxCommands)
    {
        ++numRequests;
        return pCommandAction->Action(command, index);
    }
    else
    {
        return CommandStatus::TOO_MANY_OPS;
    }
}

template<class Target, class IndexType>
IINField CommandResponseHandler::RespondToHeader(QualifierCode qualifier,
                                                 const DNP3Serializer<Target>& serializer,
                                                 const ICollection<Indexed<Target>>& values)
{
    if (pWriter)
    {
        auto iter = pWriter->IterateOverCountWithPrefix<IndexType, Target>(qualifier, serializer);
        return this->RespondToHeaderWithIterator<Target, IndexType>(qualifier, serializer, values, &iter);
    }
    else
    {
        return this->RespondToHeaderWithIterator<Target, IndexType>(qualifier, serializer, values);
    }
}

} // namespace opendnp3

#endif
