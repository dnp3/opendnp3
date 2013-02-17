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
#include "com_automatak_dnp3_impl_StackBase.h"

#include <opendnp3/IStack.h>
#include <opendnp3/VtoRouterSettings.h>

#include "JNIHelpers.hpp"

using namespace opendnp3;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_StackBase_get_1tcpclient_1vto_1endpoint
  (JNIEnv* apEnv, jobject, jlong nativeStack, jstring loggerId, jint logLevel, jstring host, jint port, jbyte channel, jlong minRetry, jlong maxRetry, jboolean startLocal, jboolean disableExt)
{
	auto pStack = (IStack*) nativeStack;
	VtoRouterSettings settings(channel, minRetry, maxRetry, startLocal, disableExt);
	std::string logger = JNIHelpers::GetString(loggerId, apEnv);
	FilterLevel level = LogTypes::ConvertIntToFilterLevel(logLevel);
	std::string address = JNIHelpers::GetString(host, apEnv);
	return (jlong) pStack->StartVtoRouterTCPClient(logger, level, address, port, settings); 
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_StackBase_get_1tcpserver_1vto_1endpoint
  (JNIEnv* apEnv, jobject, jlong nativeStack, jstring loggerId, jint logLevel, jstring adapter, jint port, jbyte channel, jlong minRetry, jlong maxRetry, jboolean startLocal, jboolean disableExt)
{
	auto pStack = (IStack*) nativeStack;
	VtoRouterSettings settings(channel, minRetry, maxRetry, startLocal, disableExt);
	std::string logger = JNIHelpers::GetString(loggerId, apEnv);
	FilterLevel level = LogTypes::ConvertIntToFilterLevel(logLevel);
	std::string address = JNIHelpers::GetString(adapter, apEnv);
	return (jlong) pStack->StartVtoRouterTCPServer(logger, level, address, port, settings);
}


