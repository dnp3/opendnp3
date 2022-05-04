/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_UPDATES_H
#define OPENDNP3_UPDATES_H

#include "opendnp3/outstation/IUpdateHandler.h"

#include <functional>
#include <memory>
#include <vector>

namespace opendnp3
{

typedef std::function<void(IUpdateHandler&)> update_func_t;
typedef std::vector<update_func_t> shared_updates_t;

class Updates
{
    friend class UpdateBuilder;

public:
    void Apply(IUpdateHandler& handler) const
    {
        if (!updates)
            return;

        for (auto& update : *updates)
        {
            update(handler);
        }
    }

    bool IsEmpty() const
    {
        return updates ? updates->empty() : true;
    }

private:
    Updates(std::shared_ptr<shared_updates_t> updates) : updates(std::move(updates)) {}

    const std::shared_ptr<shared_updates_t> updates;
};

} // namespace opendnp3

#endif
