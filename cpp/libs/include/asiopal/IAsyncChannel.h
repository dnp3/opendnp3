/*
 * Copyright 2013-2019 Automatak, LLC
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
#ifndef ASIOPAL_IASYNCCHANNEL_H
#define ASIOPAL_IASYNCCHANNEL_H

#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>
#include <openpal/util/Uncopyable.h>

#include "asiopal/Executor.h"
#include "asiopal/IChannelCallbacks.h"

#include <functional>
#include <memory>

namespace asiopal
{

class IAsyncChannel : public std::enable_shared_from_this<IAsyncChannel>, private openpal::Uncopyable
{
public:
    IAsyncChannel(const std::shared_ptr<Executor>& executor) : executor(executor) {}

    virtual ~IAsyncChannel() {}

    void SetCallbacks(const std::shared_ptr<IChannelCallbacks>& callbacks)
    {
        assert(callbacks);
        this->callbacks = callbacks;
    }

    inline bool BeginRead(const openpal::WSlice& buffer)
    {
        assert(callbacks);
        if (this->CanRead())
        {
            this->reading = true;
            this->BeginReadImpl(buffer);
            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool BeginWrite(const openpal::RSlice& buffer)
    {
        assert(callbacks);
        if (this->CanWrite())
        {
            this->writing = true;
            this->BeginWriteImpl(buffer);
            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool Shutdown()
    {
        if (this->is_shutting_down)
            return false;

        this->is_shutting_down = true;

        this->ShutdownImpl();

        // keep the channel alive until it's not reading or writing
        auto action = [self = shared_from_this()]() { self->CheckForShutdown(self); };

        this->executor->strand.post(action);

        return true;
    }

    inline bool CanRead() const
    {
        return callbacks && !is_shutting_down && !reading;
    }

    inline bool CanWrite() const
    {
        return callbacks && !is_shutting_down && !writing;
    }

    const std::shared_ptr<Executor> executor;

protected:
    inline void OnReadCallback(const std::error_code& ec, size_t num)
    {
        this->reading = false;
        if (this->callbacks && !is_shutting_down)
        {
            this->callbacks->OnReadComplete(ec, num);
        }
    }

    inline void OnWriteCallback(const std::error_code& ec, size_t num)
    {
        this->writing = false;
        if (this->callbacks && !is_shutting_down)
        {
            this->callbacks->OnWriteComplete(ec, num);
        }
    }

private:
    void CheckForShutdown(std::shared_ptr<IAsyncChannel> self)
    {
        if (self->reading || self->writing)
        {
            auto action = [self]() { self->CheckForShutdown(self); };

            self->executor->strand.post(action);
        }
        else
        {
            self->callbacks.reset(); // drop the callbacks
        }
    }

    std::shared_ptr<IChannelCallbacks> callbacks;

    bool is_shutting_down = false;
    bool reading = false;
    bool writing = false;

    virtual void BeginReadImpl(openpal::WSlice buffer) = 0;
    virtual void BeginWriteImpl(const openpal::RSlice& buffer) = 0;
    virtual void ShutdownImpl() = 0;
};

} // namespace asiopal

#endif
