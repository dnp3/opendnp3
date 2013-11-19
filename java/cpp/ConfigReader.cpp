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
#include "ConfigReader.hpp"


#include "JNIHelpers.hpp"

#include <iostream>
#include <assert.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterStackConfig cfg;

	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;"));
	cfg.master = ConvertMasterConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "masterConfig", "Lcom/automatak/dnp3/MasterConfig;"));

	return cfg;
}

SlaveStackConfig ConfigReader::ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveStackConfig cfg;

	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;"));
	cfg.slave = ConvertOutstationConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;"));
	cfg.device = ConvertDatabaseConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;"));

	return cfg;
}

SlaveConfig ConfigReader::ConvertOutstationConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveConfig cfg;

	cfg.mMaxControls  = JNIHelpers::GetIntField(apEnv, jCfg, "maxControls");
	cfg.mDisableUnsol = JNIHelpers::GetBoolField(apEnv, jCfg, "disableUnsol");
	cfg.mUnsolMask  = ClassMask(JNIHelpers::GetIntField(apEnv, jCfg, "unsolMask"));
	cfg.mAllowTimeSync = JNIHelpers::GetBoolField(apEnv, jCfg, "allowTimeSync");
	cfg.mTimeSyncPeriod  = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "timeSyncPeriodMs"));
	cfg.mUnsolPackDelay  = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "unsolPackDelayMs"));
	cfg.mUnsolRetryDelay  = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "unsolRetryDelayMs"));
	cfg.mSelectTimeout = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "selectTimeoutMs"));
	cfg.mMaxFragSize  = JNIHelpers::GetIntField(apEnv, jCfg, "maxFragSize");
	cfg.mVtoWriterQueueSize = JNIHelpers::GetIntField(apEnv, jCfg, "vtoWriterQueueSize");

	jint maxBinaryEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxBinaryEvents");
	jint maxAnalogEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxAnalogEvents");
	jint maxCounterEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxCounterEvents");
	jint maxVtoEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxVtoEvents");

	cfg.mEventMaxConfig = EventMaxConfig(maxBinaryEvents, maxAnalogEvents, maxCounterEvents, maxVtoEvents);


	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticBinaryInput", "Lcom/automatak/dnp3/StaticBinaryResponse;");
		cfg.mStaticBinary = ConvertStaticBinary(GetEnumId(apEnv, jenum));
	}


	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticAnalogInput", "Lcom/automatak/dnp3/StaticAnalogResponse;");
		cfg.mStaticAnalog = ConvertStaticAnalog(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticCounter", "Lcom/automatak/dnp3/StaticCounterResponse;");
		cfg.mStaticCounter = ConvertStaticCounter(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticAnalogOutputStatus", "Lcom/automatak/dnp3/StaticAnalogOutputStatusResponse;");
		cfg.mStaticSetpointStatus = ConvertStaticAnalogOutputStatus(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventBinaryInput", "Lcom/automatak/dnp3/EventBinaryResponse;");
		cfg.mEventBinary = ConvertEventBinary(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventAnalogInput", "Lcom/automatak/dnp3/EventAnalogResponse;");
		cfg.mEventAnalog = ConvertEventAnalog(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventCounter", "Lcom/automatak/dnp3/EventCounterResponse;");
		cfg.mEventCounter = ConvertEventCounter(GetEnumId(apEnv, jenum));
	}

	return cfg;
}

jint ConfigReader::GetEnumId(JNIEnv* apEnv, jobject jenum)
{
	jmethodID mid = JNIHelpers::GetMethodID(apEnv, jenum, "getId", "()I");
	return apEnv->CallIntMethod(jenum, mid);
}


opendnp3::StaticBinaryResponse ConfigReader::ConvertStaticBinary(jint value)
{
	switch(value) {
	default:
		return SBR_GROUP1_VAR2;
	}
}

opendnp3::StaticAnalogResponse ConfigReader::ConvertStaticAnalog(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::SAR_GROUP30_VAR1;
	case(1):
		return opendnp3::SAR_GROUP30_VAR2;
	case(2):
		return opendnp3::SAR_GROUP30_VAR3;
	case(3):
		return opendnp3::SAR_GROUP30_VAR4;
	case(4):
		return opendnp3::SAR_GROUP30_VAR5;
	default:
		return opendnp3::SAR_GROUP30_VAR6;
	}
}

opendnp3::StaticCounterResponse ConfigReader::ConvertStaticCounter(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::SCR_GROUP20_VAR1;
	case(1):
		return opendnp3::SCR_GROUP20_VAR2;
	case(2):
		return opendnp3::SCR_GROUP20_VAR5;
	default:
		return opendnp3::SCR_GROUP20_VAR6;
	}
}

opendnp3::StaticSetpointStatusResponse ConfigReader::ConvertStaticAnalogOutputStatus(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::SSSR_GROUP40_VAR1;
	case(1):
		return opendnp3::SSSR_GROUP40_VAR2;
	case(2):
		return opendnp3::SSSR_GROUP40_VAR3;
	default:
		return opendnp3::SSSR_GROUP40_VAR4;
	}
}

opendnp3::EventBinaryResponse ConfigReader::ConvertEventBinary(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::EBR_GROUP2_VAR1;
	default:
		return opendnp3::EBR_GROUP2_VAR2;
	}
}

opendnp3::EventAnalogResponse ConfigReader::ConvertEventAnalog(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::EAR_GROUP32_VAR1;
	case(1):
		return opendnp3::EAR_GROUP32_VAR2;
	case(2):
		return opendnp3::EAR_GROUP32_VAR3;
	case(3):
		return opendnp3::EAR_GROUP32_VAR4;
	case(4):
		return opendnp3::EAR_GROUP32_VAR5;
	case(5):
		return opendnp3::EAR_GROUP32_VAR6;
	case(6):
		return opendnp3::EAR_GROUP32_VAR7;
	default:
		return opendnp3::EAR_GROUP32_VAR8;
	}
}

opendnp3::EventCounterResponse ConfigReader::ConvertEventCounter(jint value)
{
	switch(value) {
	case(0):
		return opendnp3::ECR_GROUP22_VAR1;
	case(1):
		return opendnp3::ECR_GROUP22_VAR2;
	case(2):
		return opendnp3::ECR_GROUP22_VAR5;
	default:
		return opendnp3::ECR_GROUP22_VAR6;
	}
}

DeviceTemplate ConfigReader::ConvertDatabaseConfig(JNIEnv* apEnv, jobject jCfg)
{
	DeviceTemplate cfg;

	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.mBinary.push_back(EventPointRecord(IntToPointClass(mask)));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			double db = JNIHelpers::GetDoubleField(apEnv, record, "deadband");
			cfg.mAnalog.push_back(DeadbandPointRecord(IntToPointClass(mask), db));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "counterInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.mCounter.push_back(EventPointRecord(IntToPointClass(mask)));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryOutputStatii", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			cfg.mControlStatus.push_back(PointRecord());
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogOutputStatii", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			cfg.mSetpointStatus.push_back(PointRecord());
		});
	}

	return cfg;
}


MasterConfig ConfigReader::ConvertMasterConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterConfig cfg;

	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxRequestFragmentSize");
	cfg.UseNonStandardVtoFunction = JNIHelpers::GetBoolField(apEnv, jCfg, "useNonStandardVtoFunction");
	cfg.AllowTimeSync = JNIHelpers::GetBoolField(apEnv, jCfg, "allowTimeSync");
	cfg.DoUnsolOnStartup = JNIHelpers::GetBoolField(apEnv, jCfg, "doUnsolOnStartup");
	cfg.EnableUnsol = JNIHelpers::GetBoolField(apEnv, jCfg, "enableUnsol");
	cfg.UnsolClassMask = JNIHelpers::GetIntField(apEnv, jCfg, "unsolClassMask");
	cfg.IntegrityRate = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "integrityRateMs"));
	cfg.TaskRetryRate = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "taskRetryRateMs"));

	jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "scans", "Ljava/util/List;");

	JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject scan) {
		int mask = JNIHelpers::GetIntField(apEnv, scan, "classMask");
		long rate = JNIHelpers::GetLongField(apEnv, scan, "scanRateMs");
		cfg.AddExceptionScan(mask, TimeDuration::Milliseconds(rate));
	});

	return cfg;
}

AppConfig ConfigReader::ConvertAppConfig(JNIEnv* apEnv, jobject jCfg)
{
	AppConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	cfg.RspTimeout = TimeDuration::Milliseconds(JNIHelpers::GetLongField(apEnv, jCfg, "rspTimeoutMs"));
	cfg.NumRetry = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry");
	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxFragSize");

	return cfg;
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg)
{
	LinkConfig cfg(true, false);

	cfg.IsMaster = JNIHelpers::GetBoolField(apEnv, jCfg, "isMaster");
	cfg.UseConfirms = JNIHelpers::GetBoolField(apEnv, jCfg, "useConfirms");
	cfg.NumRetry = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry");
	cfg.LocalAddr = JNIHelpers::GetIntField(apEnv, jCfg, "localAddr");
	cfg.RemoteAddr = JNIHelpers::GetIntField(apEnv, jCfg, "remoteAddr");

	return cfg;
}



