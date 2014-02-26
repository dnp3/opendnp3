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
	cfg.database = ConvertDatabaseConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;"));

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

	jint maxBinaryEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxBinaryEvents");
	jint maxAnalogEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxAnalogEvents");
	jint maxCounterEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxCounterEvents");
	jint maxVtoEvents = JNIHelpers::GetIntField(apEnv, jCfg, "maxVtoEvents");

	cfg.mEventMaxConfig = EventMaxConfig(maxBinaryEvents, maxAnalogEvents, maxCounterEvents, maxVtoEvents);


	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticBinaryInput", "Lcom/automatak/dnp3/StaticBinaryResponse;");
		cfg.mStaticBinary = StaticBinaryResponseFromType(GetEnumId(apEnv, jenum));
	}


	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticAnalogInput", "Lcom/automatak/dnp3/StaticAnalogResponse;");
		cfg.mStaticAnalog = StaticAnalogResponseFromType(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticCounter", "Lcom/automatak/dnp3/StaticCounterResponse;");
		cfg.mStaticCounter = StaticCounterResponseFromType(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "staticAnalogOutputStatus", "Lcom/automatak/dnp3/StaticAnalogOutputStatusResponse;");
		cfg.mStaticAnalogOutputStatus = StaticAnalogOutputStatusResponseFromType(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventBinaryInput", "Lcom/automatak/dnp3/EventBinaryResponse;");
		cfg.mEventBinary = EventBinaryResponseFromType(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventAnalogInput", "Lcom/automatak/dnp3/EventAnalogResponse;");
		cfg.mEventAnalog = EventAnalogResponseFromType(GetEnumId(apEnv, jenum));
	}

	{
		jobject jenum = JNIHelpers::GetObjectField(apEnv, jCfg, "eventCounter", "Lcom/automatak/dnp3/EventCounterResponse;");
		cfg.mEventCounter = EventCounterResponseFromType(GetEnumId(apEnv, jenum));
	}

	return cfg;
}

jint ConfigReader::GetEnumId(JNIEnv* apEnv, jobject jenum)
{
	jmethodID mid = JNIHelpers::GetMethodID(apEnv, jenum, "toType", "()I");
	return apEnv->CallIntMethod(jenum, mid);
}

DatabaseConfiguration ConfigReader::ConvertDatabaseConfig(JNIEnv* apEnv, jobject jCfg)
{
	DatabaseConfiguration cfg;

	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.binaryMetadata.push_back(EventPointRecord(IntToPointClass(mask)));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			double db = JNIHelpers::GetDoubleField(apEnv, record, "deadband");
			cfg.analogMetadata.push_back(DeadbandPointRecord<double>(IntToPointClass(mask), db));
		});
	}
	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "counterInputs", "Ljava/util/List;");
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			int mask = JNIHelpers::GetIntField(apEnv, record, "pointClass");
			cfg.counterMetadata.push_back(DeadbandPointRecord<uint32_t>(IntToPointClass(mask), 0));
		});
	}
	
	// TODO - make all types exposed with events

	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "binaryOutputStatii", "Ljava/util/List;");
		uint32_t numBOStatus = 0;
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			++numBOStatus;
		});
		cfg.numBinaryOutputStatus = numBOStatus;
	}

	{
		jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "analogOutputStatii", "Ljava/util/List;");
		uint32_t numSStatus = 0;
		JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject record) {
			++numSStatus;
		});
		cfg.numAnalogOutputStatus = numSStatus;
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



