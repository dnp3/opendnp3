/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include "ConfigReader.h"

#include "../jni/JCache.h"

#include <iostream>

using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;

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
	cfg.timeSyncMode = static_cast<TimeSyncMode>(jni::JCache::TimeSyncMode.toType(env, config.gettimeSyncMode(env, jcfg)));
	cfg.disableUnsolOnStartup = !!config.getdisableUnsolOnStartup(env, jcfg);
	cfg.ignoreRestartIIN = !!config.getignoreRestartIIN(env, jcfg);
	cfg.unsolClassMask = ConvertClassField(env, config.getunsolClassMask(env, jcfg));
	cfg.startupIntegrityClassMask = ConvertClassField(env, config.getstartupIntegrityClassMask(env, jcfg));
	cfg.integrityOnEventOverflowIIN = !!config.getintegrityOnEventOverflowIIN(env, jcfg);
	cfg.taskRetryPeriod = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskRetryPeriod(env, jcfg)));
	cfg.taskStartTimeout = TimeDuration::Milliseconds(dur.toMillis(env, config.gettaskStartTimeout(env, jcfg)));
	cfg.maxTxFragSize = config.getmaxTxFragSize(env, jcfg);
	cfg.maxRxFragSize = config.getmaxRxFragSize(env, jcfg);

	return cfg;
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* env, jobject jlinkcfg)
{
	LinkConfig cfg(true, false);

	auto& ref = jni::JCache::LinkLayerConfig;

	cfg.IsMaster = !!ref.getisMaster(env, jlinkcfg);
	cfg.UseConfirms = !!ref.getuseConfirms(env, jlinkcfg);
	cfg.NumRetry = ref.getnumRetry(env, jlinkcfg);
	cfg.LocalAddr = static_cast<uint16_t>(ref.getlocalAddr(env, jlinkcfg));
	cfg.RemoteAddr = static_cast<uint16_t>(ref.getremoteAddr(env, jlinkcfg));
	cfg.Timeout = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getresponseTimeout(env, jlinkcfg)));
	cfg.KeepAliveTimeout = TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, ref.getkeepAliveTimeout(env, jlinkcfg)));

	return cfg;
}

OutstationStackConfig ConfigReader::ConvertOutstationStackConfig(JNIEnv* env, jobject jconfig)
{
	auto& cfg = jni::JCache::OutstationStackConfig;
	auto& list = jni::JCache::List;
	auto& db = jni::JCache::DatabaseConfig;

	const auto jdb = cfg.getdatabaseConfig(env, jconfig);

	OutstationStackConfig config(
	    DatabaseSizes(
	        static_cast<uint16_t>(list.size(env, db.getbinary(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getdoubleBinary(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getanalog(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getcounter(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getfrozenCounter(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getboStatus(env, jdb))),
	        static_cast<uint16_t>(list.size(env, db.getaoStatus(env, jdb))),
	        0
	    )
	);

	config.link = ConvertLinkConfig(env, cfg.getlinkConfig(env, jconfig));
	config.outstation.eventBufferConfig = ConvertEventBufferConfig(env, cfg.geteventBufferConfig(env, jconfig));
	config.outstation.params = ConvertOutstationConfig(env, cfg.getoutstationConfig(env, jconfig));

	ConvertDatabase(env, cfg.getdatabaseConfig(env, jconfig), config.dbConfig);

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
	           static_cast<uint16_t>(jni::JCache::EventBufferConfig.getmaxAnalogOutputStatusEvents(env, jeventconfig))
	       );
}

opendnp3::OutstationParams ConfigReader::ConvertOutstationConfig(JNIEnv* env, jobject jconfig)
{
	opendnp3::OutstationParams config;

	auto& cfg = jni::JCache::OutstationConfig;

	config.indexMode = static_cast<IndexMode>(jni::JCache::IndexMode.toType(env, cfg.getindexMode(env, jconfig)));
	config.maxControlsPerRequest = static_cast<uint8_t>(cfg.getmaxControlsPerRequest(env, jconfig));
	config.selectTimeout = ConvertDuration(env, cfg.getselectTimeout(env, jconfig));
	config.solConfirmTimeout = ConvertDuration(env, cfg.getsolConfirmTimeout(env, jconfig));
	config.unsolRetryTimeout = ConvertDuration(env, cfg.getunsolRetryTimeout(env, jconfig));
	config.maxTxFragSize = cfg.getmaxTxFragSize(env, jconfig);
	config.maxRxFragSize = cfg.getmaxRxFragSize(env, jconfig);
	config.allowUnsolicited = !!cfg.getallowUnsolicited(env, jconfig);

	return config;
}

openpal::TimeDuration ConfigReader::ConvertDuration(JNIEnv* env, jobject jduration)
{
	return openpal::TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, jduration));
}

void ConfigReader::ConvertDatabase(JNIEnv* env, jobject jdb, asiodnp3::DatabaseConfig& cfg)
{
	auto& db = jni::JCache::DatabaseConfig;
}



