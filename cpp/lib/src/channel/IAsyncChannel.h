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
#ifndef OPENDNP3_IASYNCCHANNEL_H
#define OPENDNP3_IASYNCCHANNEL_H

#include "channel/IChannelCallbacks.h"

#include "opendnp3/util/Uncopyable.h"

#include <ser4cpp/container/SequenceTypes.h>

#include <exe4cpp/asio/StrandExecutor.h>

#include <cassert>
#include <memory>

namespace opendnp3
{

class IAsyncChannel : public std::enable_shared_from_this<IAsyncChannel>, private Uncopyable
{
public:
    IAsyncChannel(const std::shared_ptr<exe4cpp::StrandExecutor>& executor) : executor(executor) {}

    virtual ~IAsyncChannel() {}

    void SetCallbacks(const std::shared_ptr<IChannelCallbacks>& callbacks)
    {
        assert(callbacks);
        this->callbacks = callbacks;
    }

    inline bool BeginRead(const ser4cpp::wseq_t& buffer)
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

    inline bool BeginWrite(const ser4cpp::rseq_t& buffer)
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

        this->executor->post(action);

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

    const std::shared_ptr<exe4cpp::StrandExecutor> executor;

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

            self->executor->post(action);
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

    virtual void BeginReadImpl(ser4cpp::wseq_t buffer) = 0;
    virtual void BeginWriteImpl(const ser4cpp::rseq_t& buffer) = 0;
    virtual void ShutdownImpl() = 0;
};

} // namespace opendnp3

#endif
