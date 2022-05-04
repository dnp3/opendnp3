/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
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

#include "LogHandlerAdapter.h"

#include "../jni/JCache.h"

using namespace jni;

LogHandlerAdapter::LogHandlerAdapter(jni::JLogHandler proxy) : proxy(proxy) {}

void LogHandlerAdapter::log(opendnp3::ModuleId module,
                            const char* id,
                            opendnp3::LogLevel level,
                            char const* location,
                            char const* message)
{
    const auto env = JNI::GetEnv();    

    LocalJString jid(env, id);
    LocalJString jlocation(env, location);
    LocalJString jmessage(env, message);

    auto jentry = JCache::LogEntry.construct(env, level.value, jid.get(), jlocation.get(), jmessage.get());

    JCache::LogHandler.log(env, proxy, jentry);
}
