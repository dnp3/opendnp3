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
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include "JNIHelpers.hpp"
#include "LogSubscriberAdapter.hpp"

#include <asiodnp3/ASIODNP3Manager.h>
#include <openpal/Exception.h>

using namespace asiodnp3;
using namespace openpal;

#include <assert.h>
#include <iostream>

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
(JNIEnv* apEnv, jobject, jint concurrency)
{
	JavaVM* pJVM;
	apEnv->GetJavaVM(&pJVM);
	assert(pJVM != NULL);
	auto pManager = new ASIODNP3Manager(concurrency,
	[pJVM]() {
		JNIHelpers::AttachThread(pJVM);
	},
	[pJVM]() {
		JNIHelpers::DetachThread(pJVM);
	});
	return (jlong) pManager;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_destroy_1native_1manager
(JNIEnv*, jobject, jlong ptrManager)
{
	delete (ASIODNP3Manager*) ptrManager;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client
(JNIEnv* pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jhost, jint port)
{
	try {
		auto pMgr = (ASIODNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string host = JNIHelpers::GetString(jhost, pEnv);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddTCPClient(Logger(pMgr->GetLog(), lev, loggerId), TimeDuration::Milliseconds(timeoutMs), host, port);
	}
	catch(Exception ex) {
		MACRO_RETHROW_EXCEPTION(pEnv, ex);
		return NULL;
	}
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server
(JNIEnv* pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jendpoint, jint port)
{
	try {
		auto pMgr = (ASIODNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string endpoint = JNIHelpers::GetString(jendpoint, pEnv);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddTCPServer(Logger(pMgr->GetLog(), lev, loggerId), TimeDuration::Milliseconds(timeoutMs), endpoint, port);
	}
	catch(Exception ex) {
		MACRO_RETHROW_EXCEPTION(pEnv, ex)
		return NULL;
	}
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial
(JNIEnv* pEnv, jobject, jlong ptrManager, jstring jloggerId, jint logLevel, jlong timeoutMs, jstring jport, jint baudRate, jint dataBits, jint parity, jint stopBits, jint flowControl)
{
	try {
		auto pMgr = (ASIODNP3Manager*) ptrManager;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		std::string port = JNIHelpers::GetString(jport, pEnv);
		asiopal::SerialSettings ss;
		ss.mDevice = port;
		ss.mBaud = baudRate;
		ss.mDataBits = dataBits;
		ss.mStopBits = stopBits;
		ss.mParity = asiopal::GetParityFromInt(parity);
		ss.mFlowType = asiopal::GetFlowTypeFromInt(flowControl);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		return (jlong) pMgr->AddSerial(Logger(pMgr->GetLog(), lev, loggerId), TimeDuration::Milliseconds(timeoutMs), ss);
	}
	catch(Exception ex) {
		MACRO_RETHROW_EXCEPTION(pEnv, ex)
		return NULL;
	}
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_native_1add_1log_1subscriber
(JNIEnv* pEnv, jobject, jlong ptrManager, jobject jproxy)
{
	auto pMgr = (ASIODNP3Manager*) ptrManager;
	JavaVM* pJVM;
	pEnv->GetJavaVM(&pJVM);
	assert(pJVM != NULL);
	jobject global = pEnv->NewGlobalRef(jproxy);
	pMgr->AddDestructorHook([pJVM, global]() {
		JNIHelpers::DeleteGlobalReference(pJVM, global);
	});
	auto pSub = new LogSubscriberAdapter(pJVM, global);
	pMgr->AddDestructorHook([pSub]() {
		delete pSub;
	});
	pMgr->AddLogSubscriber(pSub);
}
