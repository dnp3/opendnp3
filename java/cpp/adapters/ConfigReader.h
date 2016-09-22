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
#ifndef __CONFIG_READER_H_
#define __CONFIG_READER_H_

#include <jni.h>
#include <string>

#include <opendnp3/master/MasterStackConfig.h>

namespace classes
{
	static const char* duration = "Ljava/time/Duration;";
	static const char* timeSyncMode = "Lcom/automatak/dnp3/enums/TimeSyncMode;";
}

class ConfigReader
{
public:

	static opendnp3::MasterStackConfig ConvertMasterStackConfig(JNIEnv* env, jobject jconfig);
	
	//static opendnp3::SlaveStackConfig ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jcfg);

private:

	

	static opendnp3::LinkConfig ConvertLinkConfig(JNIEnv* env, jobject jconfig);

	

	static opendnp3::MasterParams ConvertMasterConfig(JNIEnv* apEnv, jobject jcfg);

	/*
	
	
	static opendnp3::SlaveConfig ConvertOutstationConfig(JNIEnv* apEnv, jobject jcfg);
	static opendnp3::DeviceTemplate ConvertDatabaseConfig(JNIEnv* apEnv, jobject jcfg);

	static opendnp3::StaticBinaryResponse ConvertStaticBinary(jint value);
	static opendnp3::StaticAnalogResponse ConvertStaticAnalog(jint value);
	static opendnp3::StaticCounterResponse ConvertStaticCounter(jint value);
	static opendnp3::StaticSetpointStatusResponse ConvertStaticAnalogOutputStatus(jint value);

	static opendnp3::EventBinaryResponse ConvertEventBinary(jint value);
	static opendnp3::EventAnalogResponse ConvertEventAnalog(jint value);
	static opendnp3::EventCounterResponse ConvertEventCounter(jint value);
	*/

	static openpal::TimeDuration ConvertDuration(JNIEnv* env, jobject jduration);

	static jint GetEnumId(JNIEnv* apEnv, jobject jenum);


};

#endif

