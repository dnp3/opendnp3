/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
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

