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
#include "ConfigReader.h"

#include "../jni/JCache.h"
#include "JNI.h"

#include <opendnp3/app/MeasurementInfo.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::Convert(JNIEnv* env, jni::JMasterStackConfig jcfg)
{
    MasterStackConfig cfg;

    cfg.link = Convert(env, jni::JCache::MasterStackConfig.getlink(env, jcfg));
    cfg.master = Convert(env, jni::JCache::MasterStackConfig.getmaster(env, jcfg));

    return cfg;
}

MasterParams ConfigReader::Convert(JNIEnv* env, jni::JMasterConfig jcfg)
{
    MasterParams cfg;

    auto& config = jni::JCache::MasterConfig;
    auto& dur = jni::JCache::Duration;

    cfg.responseTimeout = TimeDuration::Milliseconds(dur.toMillis(env, config.getresponseTimeout(env, jcfg)));
    cfg.timeSyncMode
        = static_cast<TimeSyncMode>(jni::JCache::TimeSyncMode.toType(env, config.gettimeSyncMode(env, jcfg)));
    cfg.disableUnsolOnStartup = !(config.getdisableUnsolOnStartup(env, jcfg) == 0u);
    cfg.ignoreRestartIIN = !(config.getignoreRestartIIN(env, jcfg) == 0u);
    cfg.unsolClassMask = Convert(env, config.getunsolClassMask(env, jcfg));
    cfg.startupIntegrityClassMask = Convert(env, config.getstartupIntegrityClassMask(env, jcfg));
    cfg.integrityOnEventOverflowIIN = !(config.getintegrityOnEventOverflowIIN(env, jcfg) == 0u);
    cfg.taskRetryPeriod = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskRetryPeriod(env, jcfg)));
    cfg.taskStartTimeout = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskStartTimeout(env, jcfg)));
    cfg.maxTxFragSize = config.getmaxTxFragSize(env, jcfg);
    cfg.maxRxFragSize = config.getmaxRxFragSize(env, jcfg);
    cfg.controlQualifierMode = static_cast<IndexQualifierMode>(
        jni::JCache::IndexQualifierMode.toType(env, config.getcontrolQualifierMode(env, jcfg)));

    return cfg;
}

LinkConfig ConfigReader::Convert(JNIEnv* env, jni::JLinkLayerConfig jlinkcfg)
{
    LinkConfig cfg(true, false);

    auto& ref = jni::JCache::LinkLayerConfig;

    cfg.IsMaster = !(ref.getisMaster(env, jlinkcfg) == 0u);
    cfg.LocalAddr = static_cast<uint16_t>(ref.getlocalAddr(env, jlinkcfg));
    cfg.RemoteAddr = static_cast<uint16_t>(ref.getremoteAddr(env, jlinkcfg));
    cfg.Timeout
        = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getresponseTimeout(env, jlinkcfg)));
    cfg.KeepAliveTimeout
        = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getkeepAliveTimeout(env, jlinkcfg)));

    return cfg;
}

OutstationStackConfig ConfigReader::Convert(JNIEnv* env, jni::JOutstationStackConfig jconfig)
{
    auto& cfg = jni::JCache::OutstationStackConfig;

    OutstationStackConfig config;

    config.link = Convert(env, cfg.getlinkConfig(env, jconfig));
    config.database = Convert(env, cfg.getdatabaseConfig(env, jconfig));
    config.outstation.eventBufferConfig = Convert(env, cfg.geteventBufferConfig(env, jconfig));
    config.outstation.params = Convert(env, cfg.getoutstationConfig(env, jconfig));
            
    return config;
}

opendnp3::DatabaseConfig ConfigReader::Convert(JNIEnv* env, jni::JDatabaseConfig jdb)
{
    auto& db = jni::JCache::DatabaseConfig;  
	auto& map = jni::JCache::Map;

	auto get_index = [env](jni::JEntry entry) -> uint16_t
    { 
		const auto index = jni::JCache::Integer.intValue(env, jni::JCache::Entry.getKey(env, entry).as<jni::JInteger>());
        return static_cast<uint16_t>(index);
	};

	auto get_value = [env](jni::JEntry entry) -> LocalRef<jni::JObject> {
        return jni::JCache::Entry.getValue(env, entry);        
    };
	
	opendnp3::DatabaseConfig config;
    
    JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getbinary(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
		config.binary_input[get_index(entry)] = Convert(env, get_value(entry).as<jni::JBinaryConfig>());
    });

    JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getdoubleBinary(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.double_binary[get_index(entry)] = Convert(env, get_value(entry).as<jni::JDoubleBinaryConfig>());
    });

    JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getanalog(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.analog_input[get_index(entry)] = Convert(env, get_value(entry).as<jni::JAnalogConfig>());
    });
    
	JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getcounter(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.counter[get_index(entry)] = Convert(env, get_value(entry).as<jni::JCounterConfig>());
    });
    
	JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getfrozenCounter(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.frozen_counter[get_index(entry)] = Convert(env, get_value(entry).as<jni::JFrozenCounterConfig>());
    });
    
	JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getboStatus(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.binary_output_status[get_index(entry)] = Convert(env, get_value(entry).as<jni::JBinaryOutputStatusConfig>());
    });

    JNI::Iterate<jni::JEntry>(env, map.entrySet(env, db.getaoStatus(env, jdb)).as<jni::JIterable>(), [&](jni::JEntry entry) {
        config.analog_output_status[get_index(entry)] = Convert(env, get_value(entry).as<jni::JAnalogOutputStatusConfig>());
    });

	return config;
}

opendnp3::ClassField ConfigReader::Convert(JNIEnv* env, jni::JClassField jclassfield)
{
    jint value = jni::JCache::ClassField.getbitfield(env, jclassfield);
    return ClassField(static_cast<uint8_t>(value));
}

opendnp3::StaticTypeBitField ConfigReader::Convert(JNIEnv* env, jni::JStaticTypeBitField jbitfield)
{
    jint value = jni::JCache::StaticTypeBitField.getbitfield(env, jbitfield);
    return opendnp3::StaticTypeBitField(static_cast<uint8_t>(value));
}

opendnp3::EventBufferConfig ConfigReader::Convert(JNIEnv* env, jni::JEventBufferConfig jeventconfig)
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

opendnp3::OutstationParams ConfigReader::Convert(JNIEnv* env, jni::JOutstationConfig jconfig)
{
    opendnp3::OutstationParams config;

    auto& cfg = jni::JCache::OutstationConfig;

    config.maxControlsPerRequest = static_cast<uint32_t>(cfg.getmaxControlsPerRequest(env, jconfig));
    config.selectTimeout = Convert(env, cfg.getselectTimeout(env, jconfig));
    config.solConfirmTimeout = Convert(env, cfg.getsolConfirmTimeout(env, jconfig));
    config.unsolConfirmTimeout = Convert(env, cfg.getunsolConfirmTimeout(env, jconfig));
    config.numUnsolRetries = Convert(env, cfg.getnumUnsolRetries(env, jconfig));
    config.maxTxFragSize = cfg.getmaxTxFragSize(env, jconfig);
    config.maxRxFragSize = cfg.getmaxRxFragSize(env, jconfig);
    config.allowUnsolicited = !(cfg.getallowUnsolicited(env, jconfig) == 0u);
    config.typesAllowedInClass0 = Convert(env, cfg.gettypesAllowedInClass0(env, jconfig));
    config.noDefferedReadDuringUnsolicitedNullResponse = !(cfg.getnoDefferedReadDuringUnsolicitedNullResponse(env, jconfig) == 0u);

    return config;
}

opendnp3::TimeDuration ConfigReader::Convert(JNIEnv* env, jni::JDuration jduration)
{
    return opendnp3::TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, jduration));
}

opendnp3::NumRetries ConfigReader::Convert(JNIEnv* env, jni::JNumRetries jnumretries)
{
    if(jni::JCache::NumRetries.getisInfinite(env, jnumretries))
    {
        return opendnp3::NumRetries::Infinite();
    }
    else
    {
        return opendnp3::NumRetries::Fixed(jni::JCache::NumRetries.getmaxNumRetries(env, jnumretries));
    }
}

template<class Info, class ConfigType, class ConfigCache, class StaticVariation, class EventVariation>
ConfigType ConvertEventType(
    JNIEnv* env, jobject jconfig, ConfigCache& cache, StaticVariation& svariation, EventVariation& evariation)
{
	ConfigType cfg;
    cfg.clazz = static_cast<PointClass>(jni::JCache::PointClass.toType(env, jni::JCache::EventConfig.getclazz(env, jconfig)));
    cfg.svariation = static_cast<typename Info::static_variation_t>(svariation.toType(env, cache.getstaticVariation(env, jconfig)));
    cfg.evariation = static_cast<typename Info::event_variation_t>(evariation.toType(env, cache.geteventVariation(env, jconfig)));
    return cfg;
}

template<class Info, class ConfigType, class ConfigCache, class StaticVariation, class EventVariation>
ConfigType ConvertDeadbandType(
    JNIEnv* env, jobject jconfig, ConfigCache& cache, StaticVariation& svariation, EventVariation& evariation)
{
    ConfigType cfg;    
    cfg.clazz = static_cast<PointClass>(jni::JCache::PointClass.toType(env, jni::JCache::EventConfig.getclazz(env, jconfig)));
    cfg.svariation = static_cast<typename Info::static_variation_t>(svariation.toType(env, cache.getstaticVariation(env, jconfig)));
    cfg.evariation = static_cast<typename Info::event_variation_t>(evariation.toType(env, cache.geteventVariation(env, jconfig)));
    cfg.deadband = cache.getdeadband(env, jconfig);
    return cfg;
}



opendnp3::BinaryConfig ConfigReader::Convert(JNIEnv* env, jni::JBinaryConfig jconfig)
{
    return ConvertEventType<BinaryInfo, BinaryConfig>(env, jconfig, jni::JCache::BinaryConfig, jni::JCache::StaticBinaryVariation,
                                        jni::JCache::EventBinaryVariation);
}


opendnp3::DoubleBitBinaryConfig ConfigReader::Convert(JNIEnv* env, jni::JDoubleBinaryConfig jconfig)
{
    return ConvertEventType<DoubleBitBinaryInfo, DoubleBitBinaryConfig>(env, jconfig, jni::JCache::DoubleBinaryConfig,
                                                 jni::JCache::StaticDoubleBinaryVariation,
                                                 jni::JCache::EventDoubleBinaryVariation);
}

opendnp3::AnalogConfig ConfigReader::Convert(JNIEnv* env, jni::JAnalogConfig jconfig)
{
    return ConvertDeadbandType<AnalogInfo, AnalogConfig>(env, jconfig, jni::JCache::AnalogConfig, jni::JCache::StaticAnalogVariation,
                                           jni::JCache::EventAnalogVariation);
}

opendnp3::CounterConfig ConfigReader::Convert(JNIEnv* env, jni::JCounterConfig jconfig)
{
    return ConvertDeadbandType<CounterInfo, CounterConfig>(env, jconfig, jni::JCache::CounterConfig,
                                            jni::JCache::StaticCounterVariation, jni::JCache::EventCounterVariation);
}

opendnp3::FrozenCounterConfig ConfigReader::Convert(JNIEnv* env, jni::JFrozenCounterConfig jconfig)
{
    return ConvertDeadbandType<FrozenCounterInfo, FrozenCounterConfig>(env, jconfig, jni::JCache::FrozenCounterConfig,
                                                  jni::JCache::StaticFrozenCounterVariation,
                                                  jni::JCache::EventFrozenCounterVariation);
}

opendnp3::BOStatusConfig ConfigReader::Convert(JNIEnv* env, jni::JBinaryOutputStatusConfig jconfig)
{
    return ConvertEventType<BinaryOutputStatusInfo, BOStatusConfig>(env, jconfig, jni::JCache::BinaryOutputStatusConfig,
                                                    jni::JCache::StaticBinaryOutputStatusVariation,
                                                    jni::JCache::EventBinaryOutputStatusVariation);
}

opendnp3::AOStatusConfig ConfigReader::Convert(JNIEnv* env, jni::JAnalogOutputStatusConfig jconfig)
{
    return ConvertEventType<AnalogOutputStatusInfo, AOStatusConfig>(env, jconfig, jni::JCache::AnalogOutputStatusConfig,
                                                    jni::JCache::StaticAnalogOutputStatusVariation,
                                                    jni::JCache::EventAnalogOutputStatusVariation);
}

