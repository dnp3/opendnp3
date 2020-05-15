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
#ifndef OPENDNP3_TYPED_COMMAND_HEADER_H
#define OPENDNP3_TYPED_COMMAND_HEADER_H

#include "app/HeaderWriter.h"
#include "master/ICommandHeader.h"

#include "opendnp3/app/Indexed.h"
#include "opendnp3/app/parsing/ICollection.h"
#include "opendnp3/gen/CommandPointState.h"
#include "opendnp3/gen/CommandStatus.h"
#include "opendnp3/master/ICommandCollection.h"

#include <algorithm>
#include <vector>

namespace opendnp3
{

template<class T> class TypedCommandHeader final : public ICommandHeader, public ICommandCollection<T>
{
    struct Record : public CommandState
    {
        Record(const Indexed<T>& pair) : CommandState(pair.index), command(pair.value) {}

        T command;
    };

public:
    TypedCommandHeader(const DNP3Serializer<T>& serializer) : serializer(serializer) {}

    // --- Implement ICommandCollection ---

    virtual ICommandCollection<T>& Add(const T& command, uint16_t index) override;

    // --- Implement ICommandHeader ----

    virtual bool AreAllSelected() const override;

    virtual bool Write(HeaderWriter&, IndexQualifierMode mode) override;

    virtual void ApplySelectResponse(QualifierCode code, const ICollection<Indexed<T>>& commands) override;

    virtual void ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<T>>& commands) override;

    // --- Implement ICollection<Indexed<CommandResponse>> ----

    virtual size_t Count() const override;

    virtual void Foreach(IVisitor<CommandState>& visitor) const override;

private:
    QualifierCode ExpectedQualfier() const
    {
        return this->use_single_byte_index ? QualifierCode::UINT8_CNT_UINT8_INDEX
                                           : QualifierCode::UINT16_CNT_UINT16_INDEX;
    }

    bool use_single_byte_index = true;
    const DNP3Serializer<T> serializer;
    std::vector<Record> records;
};

template<class T> ICommandCollection<T>& TypedCommandHeader<T>::Add(const T& command, uint16_t index)
{
    if (index > std::numeric_limits<uint8_t>::max())
    {
        this->use_single_byte_index = false;
    }

    this->records.push_back(WithIndex(command, index));
    return *this;
}

template<class T> bool TypedCommandHeader<T>::AreAllSelected() const
{
    return std::all_of(this->records.begin(), this->records.end(),
                       [](const Record& rec) -> bool { return rec.state == CommandPointState::SELECT_SUCCESS; });
}

template<class T> bool TypedCommandHeader<T>::Write(HeaderWriter& writer, IndexQualifierMode mode)
{
    if (this->records.empty())
    {
        return false;
    }

    // allow single byte indices if they're all <= 255 and the optimization is allowed
    this->use_single_byte_index &= (mode == IndexQualifierMode::allow_one_byte);

    if (this->use_single_byte_index)
    {
        auto iter = writer.IterateOverCountWithPrefix<ser4cpp::UInt8, T>(QualifierCode::UINT8_CNT_UINT8_INDEX,
                                                                         this->serializer);

        for (auto& rec : this->records)
        {
            if (!iter.Write(rec.command, static_cast<uint8_t>(rec.index)))
            {
                return false;
            }
        }

        return iter.IsValid();
    }
    else
    {
        auto iter = writer.IterateOverCountWithPrefix<ser4cpp::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX,
                                                                          this->serializer);

        for (auto& rec : this->records)
        {
            if (!iter.Write(rec.command, rec.index))
            {
                return false;
            }
        }

        return iter.IsValid();
    }
}

template<class T>
void TypedCommandHeader<T>::ApplySelectResponse(QualifierCode code, const ICollection<Indexed<T>>& commands)
{
    if (code != this->ExpectedQualfier())
    {
        return;
    }

    if (commands.Count() > this->records.size())
    {
        return;
    }

    uint32_t index = 0;

    auto visit = [&](const Indexed<T> item) -> void {
        auto& rec = this->records[index];
        ++index;

        if (item.index != rec.index)
        {
            return;
        }

        if (!item.value.ValuesEqual(rec.command))
        {
            rec.state = CommandPointState::SELECT_MISMATCH;
            return;
        }

        if (item.value.status != CommandStatus::SUCCESS)
        {
            rec.state = CommandPointState::SELECT_FAIL;
            rec.status = item.value.status;
            return;
        }

        if (rec.state == CommandPointState::INIT)
        {
            rec.state = CommandPointState::SELECT_SUCCESS;
        }
    };

    commands.ForeachItem(visit);
}

template<class T>
void TypedCommandHeader<T>::ApplyOperateResponse(QualifierCode code, const ICollection<Indexed<T>>& commands)
{
    if (code != this->ExpectedQualfier())
    {
        return;
    }

    if (commands.Count() > this->records.size())
    {
        return;
    }

    uint32_t index = 0;

    auto visit = [&](const Indexed<T> item) {
        auto& rec = this->records[index];
        ++index;

        if (item.index != rec.index)
        {
            return;
        }

        if (!item.value.ValuesEqual(rec.command))
        {
            rec.state = CommandPointState::OPERATE_FAIL;
            return;
        }

        rec.state = CommandPointState::SUCCESS;
        rec.status = item.value.status;
    };

    commands.ForeachItem(visit);
}

template<class T> size_t TypedCommandHeader<T>::Count() const
{
    return this->records.size();
}

template<class T> void TypedCommandHeader<T>::Foreach(IVisitor<CommandState>& visitor) const
{
    for (auto& rec : this->records)
    {
        visitor.OnValue(rec);
    }
}

} // namespace opendnp3

#endif
