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
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include "adapters/CString.h"
#include "adapters/ChannelListenerAdapter.h"
#include "adapters/LogHandlerAdapter.h"
#include "jni/JCache.h"

#include <asiodnp3/DNP3Manager.h>

using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

asiopal::TLSConfig ConvertTLSConfig(JNIEnv* env, jobject jconfig)
{
    auto& ref = jni::JCache::TLSConfig;

    CString peerCertFilePath(env, ref.getpeerCertFilePath(env, jconfig));
    CString localCertFilePath(env, ref.getlocalCertFilePath(env, jconfig));
    CString privateKeyFilePath(env, ref.getprivateKeyFilePath(env, jconfig));
    CString cipherList(env, ref.getcipherList(env, jconfig));

    return asiopal::TLSConfig(peerCertFilePath.str(), localCertFilePath.str(), privateKeyFilePath.str(),
                              ref.getmaxVerifyDepth(env, jconfig), !(ref.getallowTLSv10(env, jconfig) == 0u),
                              !(ref.getallowTLSv11(env, jconfig) == 0u), !(ref.getallowTLSv12(env, jconfig) == 0u),
                              cipherList.str());
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager(JNIEnv* /*env*/,
                                                                                         jobject /*unused*/,
                                                                                         jint concurreny,
                                                                                         jobject loghandler)
{
    auto attach = []() { JNI::AttachCurrentThread(); };
    auto detach = []() { JNI::DetachCurrentThread(); };

    auto adapter = std::make_shared<LogHandlerAdapter>(loghandler);

    return (jlong) new DNP3Manager(concurreny, adapter, attach, detach);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_shutdown_1native_1manager(JNIEnv* /*unused*/,
                                                                                          jobject /*unused*/,
                                                                                          jlong pointer)
{
    delete (DNP3Manager*)pointer;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jlong jminRetry,
                                                                                                   jlong jmaxRetry,
                                                                                                   jobject jremotes,
                                                                                                   jstring jadapter,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));
    auto listener = jlistener != nullptr ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    // Convert endpoints
    std::vector<asiopal::IPEndpoint> endpoints;
    auto process = [&](LocalRef<jobject> jendpoint) {
        const auto jaddress = jni::JCache::IPEndpoint.getaddress(env, jendpoint);
        CString address(env, jaddress);
        const auto port = jni::JCache::IPEndpoint.getport(env, jendpoint);
        asiopal::IPEndpoint endpoint(address.str(), static_cast<uint16_t>(port));
        endpoints.push_back(endpoint);
    };
    JNI::Iterate(env, jremotes, process);

    auto channel = manager->AddTCPClient(id.str(), jlevels, retry, endpoints, adapter.str(), listener);

    return (jlong) new std::shared_ptr<IChannel>(channel);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jint jmode,
                                                                                                   jstring jadapter,
                                                                                                   jint jport,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);

    auto listener = jlistener != nullptr ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    auto channel = manager->AddTCPServer(id.str(), jlevels, static_cast<ServerAcceptMode>(jmode), adapter.str(),
                                         static_cast<uint16_t>(jport), listener);

    return (jlong) new std::shared_ptr<IChannel>(channel);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tls_1client(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jlong jminRetry,
                                                                                                   jlong jmaxRetry,
                                                                                                   jobject jremotes,
                                                                                                   jstring jadapter,
                                                                                                   jobject jtlsconfig,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));
    auto tlsconf = ConvertTLSConfig(env, jtlsconfig);
    auto listener = jlistener != nullptr ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    // Convert endpoints
    std::vector<asiopal::IPEndpoint> endpoints;
    auto process = [&](LocalRef<jobject> jendpoint) {
        const auto jaddress = jni::JCache::IPEndpoint.getaddress(env, jendpoint);
        CString address(env, jaddress);
        const auto port = jni::JCache::IPEndpoint.getport(env, jendpoint);
        asiopal::IPEndpoint endpoint(address.str(), static_cast<uint16_t>(port));
        endpoints.push_back(endpoint);
    };
    JNI::Iterate(env, jremotes, process);

    std::error_code ec;

    auto channel = manager->AddTLSClient(id.str(), jlevels, retry, endpoints, adapter.str(), tlsconf, listener, ec);

    return (jlong) new std::shared_ptr<IChannel>(channel);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tls_1server(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jint jmode,
                                                                                                   jstring jadapter,
                                                                                                   jint jport,
                                                                                                   jobject jtlsconfig,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);

    auto tlsconf = ConvertTLSConfig(env, jtlsconfig);

    auto listener = jlistener != nullptr ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    std::error_code ec;

    auto channel = manager->AddTLSServer(id.str(), jlevels, static_cast<ServerAcceptMode>(jmode), adapter.str(),
                                         static_cast<uint16_t>(jport), tlsconf, listener, ec);

    return ec ? 0 : (jlong) new std::shared_ptr<IChannel>(channel);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial(JNIEnv* env,
                                                                                              jobject /*unused*/,
                                                                                              jlong native,
                                                                                              jstring jid,
                                                                                              jint jlevels,
                                                                                              jlong jminRetry,
                                                                                              jlong jmaxRetry,
                                                                                              jstring jsdevice,
                                                                                              jint jbaudRate,
                                                                                              jint jdatabits,
                                                                                              jint jparity,
                                                                                              jint jstopbits,
                                                                                              jint jflowcontrol,
                                                                                              jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));
    CString sdevice(env, jsdevice);

    asiopal::SerialSettings settings;
    settings.asyncOpenDelay = openpal::TimeDuration::Milliseconds(100);
    settings.baud = jbaudRate;
    settings.dataBits = jdatabits;
    settings.deviceName = sdevice;
    settings.flowType = opendnp3::FlowControlFromType(static_cast<uint8_t>(jflowcontrol));
    settings.parity = opendnp3::ParityFromType(static_cast<uint8_t>(jparity));
    settings.stopBits = opendnp3::StopBitsFromType(static_cast<uint8_t>(jstopbits));

    auto listener = jlistener != nullptr ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    auto channel = manager->AddSerial(id.str(), jlevels, retry, settings, listener);

    return (jlong) new std::shared_ptr<IChannel>(channel);
}
