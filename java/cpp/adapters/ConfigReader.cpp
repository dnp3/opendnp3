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
#include "ConfigReader.h"

#include "../jni/JCache.h"
#include "JNI.h"

#include <opendnp3/app/MeasurementInfo.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* env, jobject jcfg)
{
    MasterStackConfig cfg;

    cfg.link = ConvertLinkConfig(env, jni::JCache::MasterStackConfig.getlink(env, jcfg));
    cfg.master = ConvertMasterConfig(env, jni::JCache::MasterStackConfig.getmaster(env, jcfg));

    return cfg;
}

MasterParams ConfigReader::ConvertMasterConfig(JNIEnv* env, jobject jcfg)
{
    MasterParams cfg;

    auto& config = jni::JCache::MasterConfig;
    auto& dur = jni::JCache::Duration;

    cfg.responseTimeout = TimeDuration::Milliseconds(dur.toMillis(env, config.getresponseTimeout(env, jcfg)));
    cfg.timeSyncMode
        = static_cast<TimeSyncMode>(jni::JCache::TimeSyncMode.toType(env, config.gettimeSyncMode(env, jcfg)));
    cfg.disableUnsolOnStartup = !(config.getdisableUnsolOnStartup(env, jcfg) == 0u);
    cfg.ignoreRestartIIN = !(config.getignoreRestartIIN(env, jcfg) == 0u);
    cfg.unsolClassMask = ConvertClassField(env, config.getunsolClassMask(env, jcfg));
    cfg.startupIntegrityClassMask = ConvertClassField(env, config.getstartupIntegrityClassMask(env, jcfg));
    cfg.integrityOnEventOverflowIIN = !(config.getintegrityOnEventOverflowIIN(env, jcfg) == 0u);
    cfg.taskRetryPeriod = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskRetryPeriod(env, jcfg)));
    cfg.taskStartTimeout = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskStartTimeout(env, jcfg)));
    cfg.maxTxFragSize = config.getmaxTxFragSize(env, jcfg);
    cfg.maxRxFragSize = config.getmaxRxFragSize(env, jcfg);
    cfg.controlQualifierMode = static_cast<IndexQualifierMode>(
        jni::JCache::IndexQualifierMode.toType(env, config.getcontrolQualifierMode(env, jcfg)));

    return cfg;
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* env, jobject jlinkcfg)
{
    LinkConfig cfg(true, false);

    auto& ref = jni::JCache::LinkLayerConfig;

    cfg.IsMaster = !(ref.getisMaster(env, jlinkcfg) == 0u);
    cfg.UseConfirms = !(ref.getuseConfirms(env, jlinkcfg) == 0u);
    cfg.NumRetry = ref.getnumRetry(env, jlinkcfg);
    cfg.LocalAddr = static_cast<uint16_t>(ref.getlocalAddr(env, jlinkcfg));
    cfg.RemoteAddr = static_cast<uint16_t>(ref.getremoteAddr(env, jlinkcfg));
    cfg.Timeout
        = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getresponseTimeout(env, jlinkcfg)));
    cfg.KeepAliveTimeout
        = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getkeepAliveTimeout(env, jlinkcfg)));

    return cfg;
}

OutstationStackConfig ConfigReader::ConvertOutstationStackConfig(JNIEnv* env, jobject jconfig)
{
    auto& cfg = jni::JCache::OutstationStackConfig;
    auto& list = jni::JCache::List;
    auto& db = jni::JCache::DatabaseConfig;    

    OutstationStackConfig config;

    config.link = ConvertLinkConfig(env, cfg.getlinkConfig(env, jconfig));
    config.database = ConvertDatavaseConfig(env, cfg.getdatabaseConfig(env, jconfig));
    config.outstation.eventBufferConfig = ConvertEventBufferConfig(env, cfg.geteventBufferConfig(env, jconfig));
    config.outstation.params = ConvertOutstationConfig(env, cfg.getoutstationConfig(env, jconfig));
            
    return config;
}

opendnp3::DatabaseConfig ConfigReader::ConvertDatavaseConfig(JNIEnv* env, jobject jdb)
{
    auto& db = jni::JCache::DatabaseConfig;  
	auto& map = jni::JCache::Map;

	auto get_index = [env](const LocalRef<jobject>& entry) -> uint16_t
    { 
		const auto index = jni::JCache::Integer.intValue(env, jni::JCache::Entry.getKey(env, entry));
        return static_cast<uint16_t>(index);
	};

	auto get_value = [env](const LocalRef<jobject>& entry) -> LocalRef<jobject> {
        return jni::JCache::Entry.getValue(env, entry);        
    };
	
	opendnp3::DatabaseConfig config;
    
    JNI::Iterate(env, map.entrySet(env, db.getbinary(env, jdb)), [&](LocalRef<jobject> entry) { 
		config.binary_input[get_index(entry)] = ConvertBinaryConfig(env, get_value(entry));
    });

    JNI::Iterate(env, map.entrySet(env, db.getdoubleBinary(env, jdb)), [&](LocalRef<jobject> entry) {
        config.double_binary[get_index(entry)] = ConvertDoubleBinaryConfig(env, get_value(entry));
    });

    JNI::Iterate(env, map.entrySet(env, db.getanalog(env, jdb)), [&](LocalRef<jobject> entry) {
        config.analog_input[get_index(entry)] = ConvertAnalogConfig(env, get_value(entry));
    });
    
	JNI::Iterate(env, map.entrySet(env, db.getcounter(env, jdb)), [&](LocalRef<jobject> entry) {
        config.counter[get_index(entry)] = ConvertCounterConfig(env, get_value(entry));
    });
    
	JNI::Iterate(env, map.entrySet(env, db.getfrozenCounter(env, jdb)), [&](LocalRef<jobject> entry) {
        config.frozen_counter[get_index(entry)] = ConvertFrozenCounterConfig(env, get_value(entry));
    });
    
	JNI::Iterate(env, map.entrySet(env, db.getboStatus(env, jdb)), [&](LocalRef<jobject> entry) {
        config.binary_output_status[get_index(entry)] = ConvertBOStatusConfig(env, get_value(entry));
    });

    JNI::Iterate(env, map.entrySet(env, db.getaoStatus(env, jdb)), [&](LocalRef<jobject> entry) {
        config.analog_output_status[get_index(entry)] = ConvertAOStatusConfig(env, get_value(entry));
    });

	return config;
}

opendnp3::ClassField ConfigReader::ConvertClassField(JNIEnv* env, jobject jclassmask)
{
    jint value = jni::JCache::ClassField.getbitfield(env, jclassmask);
    return ClassField(static_cast<uint8_t>(value));
}

opendnp3::EventBufferConfig ConfigReader::ConvertEventBufferConfig(JNIEnv* env, jobject jeventconfig)
{
    return opendnp3::EventBufferConfig(
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxBinaryEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxDoubleBinaryEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxAnalogEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxCounterEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxFrozenCounterEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxBinaryOutputStatusEvents(env, jeventconfig)),
        static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxAnalogOutputStatusEvents(env, jeventconfig)));
}

opendnp3::OutstationParams ConfigReader::ConvertOutstationConfig(JNIEnv* env, jobject jconfig)
{
    opendnp3::OutstationParams config;

    auto& cfg = jni::JCache::OutstationConfig;

    config.maxControlsPerRequest = static_cast<uint8_t>(cfg.getmaxControlsPerRequest(env, jconfig));
    config.selectTimeout = ConvertDuration(env, cfg.getselectTimeout(env, jconfig));
    config.solConfirmTimeout = ConvertDuration(env, cfg.getsolConfirmTimeout(env, jconfig));
    config.unsolRetryTimeout = ConvertDuration(env, cfg.getunsolRetryTimeout(env, jconfig));
    config.maxTxFragSize = cfg.getmaxTxFragSize(env, jconfig);
    config.maxRxFragSize = cfg.getmaxRxFragSize(env, jconfig);
    config.allowUnsolicited = !(cfg.getallowUnsolicited(env, jconfig) == 0u);

    return config;
}



opendnp3::TimeDuration ConfigReader::ConvertDuration(JNIEnv* env, jobject jduration)
{
    return opendnp3::TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, jduration));
}

template<class Info, class ConfigType, class ConfigCache, class StaticVariation, class EventVariation>
typename ConfigType ConvertEventType(
    JNIEnv* env, jobject jconfig, ConfigCache& cache, StaticVariation& svariation, EventVariation& evariation)
{
	ConfigType cfg;
    cfg.clazz = static_cast<PointClass>(jni::JCache::PointClass.toType(env, jni::JCache::EventConfig.getclazz(env, jconfig)));
    cfg.svariation = static_cast<typename Info::static_variation_t>(svariation.toType(env, cache.getstaticVariation(env, jconfig)));
    cfg.evariation = static_cast<typename Info::event_variation_t>(evariation.toType(env, cache.geteventVariation(env, jconfig)));
    return cfg;
}

template<class Info, class ConfigType, class ConfigCache, class StaticVariation, class EventVariation>
typename ConfigType ConvertDeadbandType(
    JNIEnv* env, jobject jconfig, ConfigCache& cache, StaticVariation& svariation, EventVariation& evariation)
{
    ConfigType cfg;    
    cfg.clazz = static_cast<PointClass>(jni::JCache::PointClass.toType(env, jni::JCache::EventConfig.getclazz(env, jconfig)));
    cfg.svariation = static_cast<typename Info::static_variation_t>(svariation.toType(env, cache.getstaticVariation(env, jconfig)));
    cfg.evariation = static_cast<typename Info::event_variation_t>(evariation.toType(env, cache.geteventVariation(env, jconfig)));
    cfg.deadband = cache.getdeadband(env, jconfig);
    return cfg;
}



opendnp3::BinaryConfig ConfigReader::ConvertBinaryConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertEventType<BinaryInfo, BinaryConfig>(env, jconfig, jni::JCache::BinaryConfig, jni::JCache::StaticBinaryVariation,
                                        jni::JCache::EventBinaryVariation);
}


opendnp3::DoubleBitBinaryConfig ConfigReader::ConvertDoubleBinaryConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertEventType<DoubleBitBinaryInfo, DoubleBitBinaryConfig>(env, jconfig, jni::JCache::DoubleBinaryConfig,
                                                 jni::JCache::StaticDoubleBinaryVariation,
                                                 jni::JCache::EventDoubleBinaryVariation);
}

opendnp3::AnalogConfig ConfigReader::ConvertAnalogConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertDeadbandType<AnalogInfo, AnalogConfig>(env, jconfig, jni::JCache::AnalogConfig, jni::JCache::StaticAnalogVariation,
                                           jni::JCache::EventAnalogVariation);
}

opendnp3::CounterConfig ConfigReader::ConvertCounterConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertDeadbandType<CounterInfo, CounterConfig>(env, jconfig, jni::JCache::CounterConfig,
                                            jni::JCache::StaticCounterVariation, jni::JCache::EventCounterVariation);
}

opendnp3::FrozenCounterConfig ConfigReader::ConvertFrozenCounterConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertDeadbandType<FrozenCounterInfo, FrozenCounterConfig>(env, jconfig, jni::JCache::FrozenCounterConfig,
                                                  jni::JCache::StaticFrozenCounterVariation,
                                                  jni::JCache::EventFrozenCounterVariation);
}

opendnp3::BOStatusConfig ConfigReader::ConvertBOStatusConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertEventType<BinaryOutputStatusInfo, BOStatusConfig>(env, jconfig, jni::JCache::BinaryOutputStatusConfig,
                                                    jni::JCache::StaticBinaryOutputStatusVariation,
                                                    jni::JCache::EventBinaryOutputStatusVariation);
}

opendnp3::AOStatusConfig ConfigReader::ConvertAOStatusConfig(JNIEnv* env, jobject jconfig)
{
    return ConvertEventType<AnalogOutputStatusInfo, AOStatusConfig>(env, jconfig, jni::JCache::AnalogOutputStatusConfig,
                                                    jni::JCache::StaticAnalogOutputStatusVariation,
                                                    jni::JCache::EventAnalogOutputStatusVariation);
}

