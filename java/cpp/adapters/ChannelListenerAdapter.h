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
#ifndef OPENDNP3_CHANNELLISTENERADAPTER_H
#define OPENDNP3_CHANNELLISTENERADAPTER_H

#include <opendnp3/channel/IChannelListener.h>

#include "GlobalRef.h"
#include "../jni/JCache.h"
#include "../jni/JNIWrappers.h"


class ChannelListenerAdapter final : public opendnp3::IChannelListener
{
public:
    ChannelListenerAdapter(jni::JChannelListener proxy) : proxy(proxy) {}

    void OnStateChange(opendnp3::ChannelState state) override
    {
        const auto env = JNI::GetEnv();
        auto jstate = jni::JCache::ChannelState.fromType(env, static_cast<jint>(state));
        jni::JCache::ChannelListener.onStateChange(env, proxy, jstate);
    }

private:
    GlobalRef<jni::JChannelListener> proxy;
};

#endif
