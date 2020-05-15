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
#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include "adapters/CString.h"
#include "adapters/CommandHandlerAdapter.h"
#include "adapters/ConfigReader.h"
#include "adapters/MasterApplicationAdapter.h"
#include "adapters/OutstationApplicationAdapter.h"
#include "adapters/SOEHandlerAdapter.h"
#include "jni/JCache.h"

#include <opendnp3/channel/IChannel.h>

#include <memory>

using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_set_1log_1level_1native(JNIEnv* /*env*/,
                                                                                        jobject /*unused*/,
                                                                                        jlong native,
                                                                                        jint levels)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;
    (*channel)->SetLogFilters(LogLevel(levels));
}

JNIEXPORT jobject JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1statistics_1native(JNIEnv* env,
                                                                                           jobject /*unused*/,
                                                                                           jlong native)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;
    auto stats = (*channel)->GetStatistics();

    auto parserStats = jni::JCache::ParserStatistics.construct(
        env, stats.parser.numHeaderCrcError, stats.parser.numBodyCrcError, stats.parser.numLinkFrameRx,
        stats.parser.numBadLength, stats.parser.numBadFunctionCode, stats.parser.numBadFCV, stats.parser.numBadFCV);

    auto channelStats = jni::JCache::ChannelStatistics.construct(env, stats.channel.numOpen, stats.channel.numOpenFail,
                                                             stats.channel.numClose, stats.channel.numBytesRx,
                                                             stats.channel.numBytesTx, stats.channel.numLinkFrameTx);

    return env->NewGlobalRef(jni::JCache::LinkStatistics.construct(env, channelStats, parserStats).get());
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native(JNIEnv* /*env*/,
                                                                                 jobject /*unused*/,
                                                                                 jlong native)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;
    (*channel)->Shutdown();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_destroy_1native(JNIEnv* /*unused*/,
                                                                                jobject /*unused*/,
                                                                                jlong native)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;
    delete channel; // drop our reference
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master(
    JNIEnv* env, jobject /*unused*/, jlong native, jstring jid, jobject handler, jobject application, jobject jconfig)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;

    auto config = ConfigReader::Convert(env, jni::JMasterStackConfig(jconfig));
    auto soeAdapter = std::make_shared<SOEHandlerAdapter>(handler);
    auto appAdapter = std::make_shared<MasterApplicationAdapter>(application);

    CString id(env, jid);

    auto stack = (*channel)->AddMaster(id.str(), soeAdapter, appAdapter, config);

    return stack ? (jlong) new std::shared_ptr<IMaster>(stack) : 0;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1outstation(JNIEnv* env,
                                                                                         jobject /*unused*/,
                                                                                         jlong native,
                                                                                         jstring jid,
                                                                                         jobject commandHandler,
                                                                                         jobject application,
                                                                                         jobject jconfig)
{
    const auto channel = (std::shared_ptr<IChannel>*)native;

    auto config = ConfigReader::Convert(env, jni::JOutstationStackConfig(jconfig));
    auto commandHandlerAdapter = std::make_shared<CommandHandlerAdapter>(commandHandler);
    auto applicationAdapter = std::make_shared<OutstationApplicationAdapter>(application);

    CString id(env, jid);

    auto stack = (*channel)->AddOutstation(id.str(), commandHandlerAdapter, applicationAdapter, config);

    return stack ? (jlong) new std::shared_ptr<IOutstation>(stack) : 0;
}
