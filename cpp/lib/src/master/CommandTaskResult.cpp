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

#include "CommandTaskResult.h"

#include "master/ICommandHeader.h"

namespace opendnp3
{

CommandTaskResult::CommandTaskResult(TaskCompletion result, const CommandSet::HeaderVector& vector)
    : ICommandTaskResult(result), m_vector(&vector)
{
}

/// --- Implement ICollection<CommandResult> ----

size_t CommandTaskResult::Count() const
{
    size_t count = 0;
    for (auto& header : *m_vector)
    {
        count += header->Count();
    }
    return count;
}

void CommandTaskResult::Foreach(IVisitor<CommandPointResult>& visitor) const
{
    uint32_t headerIndex = 0;

    for (auto& header : *m_vector)
    {
        auto visit = [&](const CommandState& state) {
            visitor.OnValue(CommandPointResult(headerIndex, state.index, state.state, state.status));
        };

        header->ForeachItem(visit);
        ++headerIndex;
    }
}

} // namespace opendnp3
