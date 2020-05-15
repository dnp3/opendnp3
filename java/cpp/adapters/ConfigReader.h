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
#ifndef OPENDNP3_CONFIG_READER_H
#define OPENDNP3_CONFIG_READER_H

#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/outstation/OutstationStackConfig.h>

#include <jni.h>
#include <string>
#include "../jni/JNIWrappers.h"

class ConfigReader
{
public:
    static opendnp3::MasterStackConfig Convert(JNIEnv* env, jni::JMasterStackConfig jcfg);
    static opendnp3::OutstationStackConfig Convert(JNIEnv* env, jni::JOutstationStackConfig jcfg);

private:
    static opendnp3::LinkConfig Convert(JNIEnv* env, jni::JLinkLayerConfig jlinkcfg);
    static opendnp3::MasterParams Convert(JNIEnv* apEnv, jni::JMasterConfig jcfg);

    static opendnp3::ClassField Convert(JNIEnv* env, jni::JClassField jclassmask);
    static opendnp3::StaticTypeBitField Convert(JNIEnv* env, jni::JStaticTypeBitField jbitfield);
    static opendnp3::EventBufferConfig Convert(JNIEnv* env, jni::JEventBufferConfig jeventconfig);
    static opendnp3::OutstationParams Convert(JNIEnv* env, jni::JOutstationConfig jconfig);
    static opendnp3::DatabaseConfig Convert(JNIEnv* env, jni::JDatabaseConfig jdb);
    static opendnp3::TimeDuration Convert(JNIEnv* env, jni::JDuration jduration);
    static opendnp3::NumRetries Convert(JNIEnv* env, jni::JNumRetries jnumretries);
    

    static opendnp3::BinaryConfig Convert(JNIEnv* env, jni::JBinaryConfig jconfig);
    static opendnp3::DoubleBitBinaryConfig Convert(JNIEnv* env, jni::JDoubleBinaryConfig jconfig);
    static opendnp3::AnalogConfig Convert(JNIEnv* env, jni::JAnalogConfig jconfig);
    static opendnp3::CounterConfig Convert(JNIEnv* env, jni::JCounterConfig jconfig);
    static opendnp3::FrozenCounterConfig Convert(JNIEnv* env, jni::JFrozenCounterConfig jconfig);
    static opendnp3::BOStatusConfig Convert(JNIEnv* env, jni::JBinaryOutputStatusConfig jconfig);
    static opendnp3::AOStatusConfig Convert(JNIEnv* env, jni::JAnalogOutputStatusConfig jconfig);
};

#endif
