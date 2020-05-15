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
#ifndef OPENDNP3_RESOURCEMANAGER_H
#define OPENDNP3_RESOURCEMANAGER_H

#include "IResourceManager.h"

#include <memory>
#include <mutex>
#include <set>

namespace opendnp3
{

class ResourceManager final : public IResourceManager
{

public:
    static std::shared_ptr<ResourceManager> Create()
    {
        return std::make_shared<ResourceManager>();
    }

    void Detach(const std::shared_ptr<IResource>& resource) final;

    void Shutdown();

    template<class R, class T> std::shared_ptr<R> Bind(const T& create)
    {
        std::lock_guard<std::mutex> lock(this->mutex);

        if (this->is_shutting_down)
        {
            return nullptr;
        }
        else
        {
            auto item = create();
            if (item)
            {
                this->resources.insert(item);
            }
            return item;
        }
    }

private:
    std::mutex mutex;
    bool is_shutting_down = false;
    std::set<std::shared_ptr<IResource>> resources;
};

} // namespace opendnp3

#endif
