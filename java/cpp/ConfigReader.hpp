//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
#ifndef __CONFIG_READER_H_
#define __CONFIG_READER_H_

#include <jni.h>
#include <string>

#include <opendnp3/MasterStackConfig.h>
#include <opendnp3/SlaveStackConfig.h>

class ConfigReader
{
	public:
	static opendnp3::MasterStackConfig ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::SlaveStackConfig ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jCfg);	

	private:
	static opendnp3::LinkConfig ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::AppConfig ConvertAppConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::MasterConfig ConvertMasterConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::SlaveConfig ConvertOutstationConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::DeviceTemplate ConvertDatabaseConfig(JNIEnv* apEnv, jobject jCfg);
	static opendnp3::GrpVar ConvertGrpVar(JNIEnv* apEnv, jobject jCfg, const char* fieldId);
	
};

#endif

