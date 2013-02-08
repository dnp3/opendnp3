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
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include "JNIHelpers.h"
#include "LogSubscriberAdapter.h"

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/Exception.h>

using namespace opendnp3;

#include <iostream>

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
  (JNIEnv* apEnv, jobject, jint concurrency)
{
	JavaVM* pJVM;
	apEnv->GetJavaVM(&pJVM);
	assert(pJVM != NULL);
	auto pManager = new DNP3Manager(concurrency,		
		[pJVM](){ JNIHelpers::AttachThread(pJVM); }, 
		[pJVM](){ JNIHelpers::DetachThread(pJVM); }
	);	
	return (jlong) pManager;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_destroy_1native_1manager
  (JNIEnv *, jobject, jlong ptrManager)
{
	delete (DNP3Manager*) ptrManager;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client
  (JNIEnv * pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jhost, jint port)
{
	try {
		auto pMgr = (DNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string host = JNIHelpers::GetString(jhost, pEnv);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddTCPClient(loggerId, lev, timeoutMs, host, port);
	}
	catch(Exception ex)
	{
		MACRO_RETHROW_EXCEPTION(pEnv, ex);
	}
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server
  (JNIEnv* pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jendpoint, jint port)
{
	try {
		auto pMgr = (DNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string endpoint = JNIHelpers::GetString(jendpoint, pEnv);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddTCPServer(loggerId, lev, timeoutMs, endpoint, port);
	}
	catch(Exception ex)
	{
		MACRO_RETHROW_EXCEPTION(pEnv, ex)
	}
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial
  (JNIEnv * pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jport, jint baudRate, jint dataBits, jint parity, jint stopBits, jint flowControl)
{
	try {
		auto pMgr = (DNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string port = JNIHelpers::GetString(jport, pEnv);
		SerialSettings ss;
		ss.mDevice = port;
		ss.mBaud = baudRate;
		ss.mDataBits = dataBits;
		ss.mStopBits = stopBits;
		ss.mParity = GetParityFromInt(parity);
		ss.mFlowType = GetFlowTypeFromInt(flowControl);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddSerial(loggerId, lev, timeoutMs, ss);
	}
	catch(Exception ex)
	{
		MACRO_RETHROW_EXCEPTION(pEnv, ex)
	}
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_native_1add_1log_1subscriber
  (JNIEnv* pEnv, jobject, jlong ptrManager, jobject jsubscriber)
{
	auto pMgr = (DNP3Manager*) ptrManager;
	JavaVM* pJVM;
	pEnv->GetJavaVM(&pJVM);
	assert(pJVM != NULL);
	jobject global = pEnv->NewGlobalRef(jsubscriber);
	pMgr->AddDestructorHook([pJVM, global]() { JNIHelpers::DeleteGlobalReference(pJVM, global); });
	auto pSub = new LogSubscriberAdapter(pJVM, global);
	pMgr->AddDestructorHook([pSub](){ delete pSub; });
	pMgr->AddLogSubscriber(pSub);
}
