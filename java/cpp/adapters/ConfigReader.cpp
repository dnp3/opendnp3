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

#include "JNI.h"
#include "JNIStrings.h"

#include <iostream>

using namespace openpal;
using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* env, jobject jCfg)
{
	using namespace classes::MasterStackConfig;

	MasterStackConfig cfg;
		
	cfg.link = ConvertLinkConfig(env, JNI::GetObjectField(env, jCfg, fields::link, classes::LinkLayerConfig::fqcn));
	cfg.master = ConvertMasterConfig(env, JNI::GetObjectField(env, jCfg, fields::master, classes::MasterConfig::fqcn));
		
	return cfg;
}

MasterParams ConfigReader::ConvertMasterConfig(JNIEnv* env, jobject jcfg)
{
	using namespace classes::MasterConfig;

	MasterParams cfg;	

	cfg.responseTimeout = ConvertDuration(env, JNI::GetObjectField(env, jcfg, fields::responseTimeout, classes::Duration::fqcn));
	cfg.timeSyncMode = (TimeSyncMode) GetEnumId(env, JNI::GetObjectField(env, jcfg, fields::timeSyncMode, classes::TimeSyncMode::fqcn));
	cfg.disableUnsolOnStartup = JNI::GetBoolField(env, jcfg, fields::disableUnsolOnStartup);
	cfg.ignoreRestartIIN = JNI::GetBoolField(env, jcfg, fields::ignoreRestartIIN);
	cfg.unsolClassMask = ConvertClassField(env, JNI::GetObjectField(env, jcfg, fields::unsolClassMask, classes::ClassField::fqcn));
	cfg.startupIntegrityClassMask = ConvertClassField(env, JNI::GetObjectField(env, jcfg, fields::startupIntegrityClassMask, classes::ClassField::fqcn));
	cfg.integrityOnEventOverflowIIN = JNI::GetBoolField(env, jcfg, fields::integrityOnEventOverflowIIN);
	cfg.taskRetryPeriod = ConvertDuration(env, JNI::GetObjectField(env, jcfg, fields::taskRetryPeriod, classes::Duration::fqcn));
	cfg.taskStartTimeout = ConvertDuration(env, JNI::GetObjectField(env, jcfg, fields::taskStartTimeout, classes::Duration::fqcn));
	cfg.maxTxFragSize = JNI::GetIntField(env, jcfg, fields::maxTxFragSize);
	cfg.maxRxFragSize = JNI::GetIntField(env, jcfg, fields::maxRxFragSize);

	return cfg;
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* env, jobject jlinkcfg)
{
	using namespace classes::LinkLayerConfig;

	LinkConfig cfg(true, false);
	
	cfg.IsMaster = JNI::GetBoolField(env, jlinkcfg, fields::isMaster);
	cfg.UseConfirms = JNI::GetBoolField(env, jlinkcfg, fields::useConfirms);
	cfg.NumRetry = JNI::GetIntField(env, jlinkcfg, fields::numRetry);
	cfg.LocalAddr = static_cast<uint16_t>(JNI::GetIntField(env, jlinkcfg, fields::localAddr));
	cfg.RemoteAddr = static_cast<uint16_t>(JNI::GetIntField(env, jlinkcfg, fields::remoteAddr));
	cfg.Timeout = ConvertDuration(env, JNI::GetObjectField(env, jlinkcfg, fields::responseTimeout, classes::Duration::fqcn));
	cfg.KeepAliveTimeout = ConvertDuration(env, JNI::GetObjectField(env, jlinkcfg, fields::keepAliveTimeout, classes::Duration::fqcn));	
	
	return cfg;
}

/*
SlaveStackConfig ConfigReader::ConvertSlaveStackConfig(JNIEnv* env, jobject jCfg)
{
SlaveStackConfig cfg;

cfg.link = ConvertLinkConfig(env, JNIHelpers::GetObjectField(env, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
cfg.app = ConvertAppConfig(env, JNIHelpers::GetObjectField(env, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;"));
cfg.slave = ConvertOutstationConfig(env, JNIHelpers::GetObjectField(env, jCfg, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;"));
cfg.device = ConvertDatabaseConfig(env, JNIHelpers::GetObjectField(env, jCfg, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;"));

return cfg;
}

SlaveConfig ConfigReader::ConvertOutstationConfig(JNIEnv* env, jobject jCfg)
{
SlaveConfig cfg;

cfg.mMaxControls  = JNIHelpers::GetIntField(env, jCfg, "maxControls");
cfg.mDisableUnsol = JNIHelpers::GetBoolField(env, jCfg, "disableUnsol");
cfg.mUnsolMask  = ClassMask(JNIHelpers::GetIntField(env, jCfg, "unsolMask"));
cfg.mAllowTimeSync = JNIHelpers::GetBoolField(env, jCfg, "allowTimeSync");
cfg.mTimeSyncPeriod  = JNIHelpers::GetLongField(env, jCfg, "timeSyncPeriodMs");
cfg.mUnsolPackDelay  = JNIHelpers::GetLongField(env, jCfg, "unsolPackDelayMs");
cfg.mUnsolRetryDelay  = JNIHelpers::GetLongField(env, jCfg, "unsolRetryDelayMs");
cfg.mSelectTimeout = JNIHelpers::GetLongField(env, jCfg, "selectTimeoutMs");
cfg.mMaxFragSize  = JNIHelpers::GetIntField(env, jCfg, "maxFragSize");
cfg.mVtoWriterQueueSize = JNIHelpers::GetIntField(env, jCfg, "vtoWriterQueueSize");

jint maxBinaryEvents = JNIHelpers::GetIntField(env, jCfg, "maxBinaryEvents");
jint maxAnalogEvents = JNIHelpers::GetIntField(env, jCfg, "maxAnalogEvents");
jint maxCounterEvents = JNIHelpers::GetIntField(env, jCfg, "maxCounterEvents");
jint maxVtoEvents = JNIHelpers::GetIntField(env, jCfg, "maxVtoEvents");

cfg.mEventMaxConfig = EventMaxConfig(maxBinaryEvents, maxAnalogEvents, maxCounterEvents, maxVtoEvents);


{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "staticBinaryInput", "Lcom/automatak/dnp3/StaticBinaryResponse;");
cfg.mStaticBinary = ConvertStaticBinary(GetEnumId(env, jenum));
}


{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "staticAnalogInput", "Lcom/automatak/dnp3/StaticAnalogResponse;");
cfg.mStaticAnalog = ConvertStaticAnalog(GetEnumId(env, jenum));
}

{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "staticCounter", "Lcom/automatak/dnp3/StaticCounterResponse;");
cfg.mStaticCounter = ConvertStaticCounter(GetEnumId(env, jenum));
}

{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "staticAnalogOutputStatus", "Lcom/automatak/dnp3/StaticAnalogOutputStatusResponse;");
cfg.mStaticSetpointStatus = ConvertStaticAnalogOutputStatus(GetEnumId(env, jenum));
}

{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "eventBinaryInput", "Lcom/automatak/dnp3/EventBinaryResponse;");
cfg.mEventBinary = ConvertEventBinary(GetEnumId(env, jenum));
}

{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "eventAnalogInput", "Lcom/automatak/dnp3/EventAnalogResponse;");
cfg.mEventAnalog = ConvertEventAnalog(GetEnumId(env, jenum));
}

{
jobject jenum = JNIHelpers::GetObjectField(env, jCfg, "eventCounter", "Lcom/automatak/dnp3/EventCounterResponse;");
cfg.mEventCounter = ConvertEventCounter(GetEnumId(env, jenum));
}

return cfg;
}
*/

jint ConfigReader::GetEnumId(JNIEnv* env, jobject jenum)
{
	jmethodID mid = JNI::GetMethodIDFromObject(env, jenum, classes::AnyEnum::methods::toType);
	return env->CallIntMethod(jenum, mid);
}

/*
DeviceTemplate ConfigReader::ConvertDatabaseConfig(JNIEnv* env, jobject jCfg)
{
DeviceTemplate cfg;

{
jobject list = JNIHelpers::GetObjectField(env, jCfg, "binaryInputs", "Ljava/util/List;");
JNIHelpers::IterateOverListOfObjects(env, list, [&](jobject record) {
int mask = JNIHelpers::GetIntField(env, record, "pointClass");
cfg.mBinary.push_back(EventPointRecord(IntToPointClass(mask)));
});
}
{
jobject list = JNIHelpers::GetObjectField(env, jCfg, "analogInputs", "Ljava/util/List;");
JNIHelpers::IterateOverListOfObjects(env, list, [&](jobject record) {
int mask = JNIHelpers::GetIntField(env, record, "pointClass");
double db = JNIHelpers::GetDoubleField(env, record, "deadband");
cfg.mAnalog.push_back(DeadbandPointRecord(IntToPointClass(mask), db));
});
}
{
jobject list = JNIHelpers::GetObjectField(env, jCfg, "counterInputs", "Ljava/util/List;");
JNIHelpers::IterateOverListOfObjects(env, list, [&](jobject record) {
int mask = JNIHelpers::GetIntField(env, record, "pointClass");
cfg.mCounter.push_back(EventPointRecord(IntToPointClass(mask)));
});
}
{
jobject list = JNIHelpers::GetObjectField(env, jCfg, "binaryOutputStatii", "Ljava/util/List;");
JNIHelpers::IterateOverListOfObjects(env, list, [&](jobject record) {
cfg.mControlStatus.push_back(PointRecord());
});
}
{
jobject list = JNIHelpers::GetObjectField(env, jCfg, "analogOutputStatii", "Ljava/util/List;");
JNIHelpers::IterateOverListOfObjects(env, list, [&](jobject record) {
cfg.mSetpointStatus.push_back(PointRecord());
});
}

return cfg;
}
*/

openpal::TimeDuration ConfigReader::ConvertDuration(JNIEnv* env, jobject jduration)
{
	const auto clazz = JNI::GetClassForObject(env, jduration);
	const auto method = JNI::GetMethodIDFromObject(env, jduration, classes::Duration::methods::toMillis);	
	const jlong value = env->CallLongMethod(jduration, method);	

	return TimeDuration::Milliseconds(value);
}

opendnp3::ClassField ConfigReader::ConvertClassField(JNIEnv* env, jobject jclassmask)
{
	const jint value = JNI::GetIntField(env, jclassmask, classes::ClassField::fields::bitfield);
	return ClassField(static_cast<uint8_t>(value));
}



