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
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include "adapters/CString.h"
#include "adapters/ChannelListenerAdapter.h"
#include "adapters/LogHandlerAdapter.h"
#include "jni/JCache.h"

#include <opendnp3/DNP3Manager.h>

using namespace opendnp3;

opendnp3::TLSConfig ConvertTLSConfig(JNIEnv* env, jobject jconfig)
{
    auto& ref = jni::JCache::TLSConfig;

    CString peer_cert_file_path(env, ref.getpeerCertFilePath(env, jconfig));
    CString local_cert_file_path(env, ref.getlocalCertFilePath(env, jconfig));
    CString private_key_file_path(env, ref.getprivateKeyFilePath(env, jconfig));
    CString cipher_list(env, ref.getcipherList(env, jconfig));

    return opendnp3::TLSConfig(peer_cert_file_path.str(), local_cert_file_path.str(), private_key_file_path.str(),                              
                              ref.getallowTLSv10(env, jconfig) != 0,
                              ref.getallowTLSv11(env, jconfig) != 0,
                              ref.getallowTLSv12(env, jconfig) != 0,
                              ref.getallowTLSv13(env, jconfig) != 0,
                              cipher_list.str());
}

opendnp3::IPEndpoint ConvertIPEndpoint(JNIEnv* env, jni::JIPEndpoint jendpoint)
{
    const auto jaddress = jni::JCache::IPEndpoint.getaddress(env, jendpoint);
    CString address(env, jaddress);
    const auto port = jni::JCache::IPEndpoint.getport(env, jendpoint);
    return opendnp3::IPEndpoint(address.str(), static_cast<uint16_t>(port));
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager(JNIEnv* /*env*/,
                                                                                         jobject /*unused*/,
                                                                                         jint concurreny,
                                                                                         jobject loghandler)
{
    auto attach = [](uint32_t thread_id) { JNI::AttachCurrentThread(); };
    auto detach = [](uint32_t thread_id) { JNI::DetachCurrentThread(); };

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
                                                                                                   jlong jreconnectDelay,
                                                                                                   jobject jremotes,
                                                                                                   jstring jadapter,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry), TimeDuration::Milliseconds(jreconnectDelay));
    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    // Convert endpoints
    std::vector<opendnp3::IPEndpoint> endpoints;
    auto process = [&](jni::JIPEndpoint jendpoint) {
        endpoints.push_back(ConvertIPEndpoint(env, jendpoint));
    };
    JNI::Iterate<jni::JIPEndpoint>(env, jni::JIterable(jremotes), process);

    try
    {
        auto channel = manager->AddTCPClient(id.str(), LogLevel(jlevels), retry, endpoints, adapter.str(), listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jint jmode,
                                                                                                   jobject jendpoint,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    auto endpoint = ConvertIPEndpoint(env, jendpoint);

    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    try
    {
        auto channel = manager->AddTCPServer(id.str(), LogLevel(jlevels), static_cast<ServerAcceptMode>(jmode), endpoint, listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1udp
    (JNIEnv* env, jobject /*unused*/, jlong native, jstring jid, jint jlevels, jlong jminRetry, jlong jmaxRetry, jobject jlocalEndpoint, jobject jremoteEndpoint, jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    auto localEndpoint = ConvertIPEndpoint(env, jlocalEndpoint);
    auto remoteEndpoint = ConvertIPEndpoint(env, jremoteEndpoint);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));
    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    try
    {
        auto channel = manager->AddUDPChannel(id.str(), LogLevel(jlevels), retry, localEndpoint, remoteEndpoint, listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tls_1client(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jlong jminRetry,
                                                                                                   jlong jmaxRetry,
                                                                                                   jlong jreconnectDelay,
                                                                                                   jobject jremotes,
                                                                                                   jstring jadapter,
                                                                                                   jobject jtlsconfig,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    CString adapter(env, jadapter);
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry), TimeDuration::Milliseconds(jreconnectDelay));
    auto tlsconf = ConvertTLSConfig(env, jtlsconfig);
    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    // Convert endpoints
    std::vector<opendnp3::IPEndpoint> endpoints;
    auto process = [&](jni::JIPEndpoint jendpoint) {
        endpoints.push_back(ConvertIPEndpoint(env, jendpoint));
    };
    JNI::Iterate<jni::JIPEndpoint>(env, jni::JIterable(jremotes), process);

    try
    {
        auto channel = manager->AddTLSClient(id.str(), LogLevel(jlevels), retry, endpoints, adapter.str(), tlsconf, listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tls_1server(JNIEnv* env,
                                                                                                   jobject /*unused*/,
                                                                                                   jlong native,
                                                                                                   jstring jid,
                                                                                                   jint jlevels,
                                                                                                   jint jmode,
                                                                                                   jobject jendpoint,
                                                                                                   jobject jtlsconfig,
                                                                                                   jobject jlistener)
{
    const auto manager = (DNP3Manager*)native;

    CString id(env, jid);
    auto endpoint = ConvertIPEndpoint(env, jendpoint);

    auto tlsconf = ConvertTLSConfig(env, jtlsconfig);

    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    try
    {
        auto channel = manager->AddTLSServer(id.str(), LogLevel(jlevels), static_cast<ServerAcceptMode>(jmode), endpoint, tlsconf, listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial(JNIEnv* env,
                                                                                              jobject /*unused*/,
                                                                                              jlong native,
                                                                                              jstring jid,
                                                                                              jint jlevels,
                                                                                              jlong jminRetry,
                                                                                              jlong jmaxRetry,
                                                                                              jlong jreconnectDelay,
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
    ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry), TimeDuration::Milliseconds(jreconnectDelay));
    CString sdevice(env, jsdevice);

    opendnp3::SerialSettings settings;
    settings.asyncOpenDelay = opendnp3::TimeDuration::Milliseconds(100);
    settings.baud = jbaudRate;
    settings.dataBits = jdatabits;
    settings.deviceName = sdevice;
    settings.flowType = opendnp3::FlowControlSpec::from_type(static_cast<uint8_t>(jflowcontrol));
    settings.parity = opendnp3::ParitySpec::from_type(static_cast<uint8_t>(jparity));
    settings.stopBits = opendnp3::StopBitsSpec::from_type(static_cast<uint8_t>(jstopbits));

    auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

    try
    {
        auto channel = manager->AddSerial(id.str(), LogLevel(jlevels), retry, settings, listener);
        return (jlong) new std::shared_ptr<IChannel>(channel);
    }
    catch(DNP3Error& e)
    {
        auto exception = jni::JCache::DNP3Exception.construct(env, LocalJString(env, e.what()));
        env->Throw(exception.as<jthrowable>());
        return 0;
    }
}
