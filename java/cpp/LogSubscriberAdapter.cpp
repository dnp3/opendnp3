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
#include "LogSubscriberAdapter.hpp"

#include <iostream>
#include <chrono>

#include "JNIHelpers.hpp"

LogSubscriberAdapter::LogSubscriberAdapter(JavaVM* apJVM, jobject aProxy) :
	mpJVM(apJVM),
	mProxy(aProxy),
	mLogEntryClazz(NULL),
	mLogEntryInitMid(NULL)
{
	auto pEnv = JNIHelpers::GetEnvFromJVM(mpJVM);
	mLogEntryClazz = pEnv->FindClass("com/automatak/dnp3/impl/LogEntryImpl");
	assert(mLogEntryClazz != NULL);
	mLogEntryInitMid = pEnv->GetMethodID(mLogEntryClazz, "<init>", "(ILjava/lang/String;Ljava/lang/String;JI)V");
	assert(mLogEntryInitMid != NULL);
}

void LogSubscriberAdapter::Log(const opendnp3::LogEntry& arEntry)
{
	JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(mpJVM);

	jint level = arEntry.GetFilterLevel();
	jstring name = pEnv->NewStringUTF(arEntry.GetDeviceName().c_str());
	jstring msg = pEnv->NewStringUTF(arEntry.GetMessage().c_str());
	jlong time = std::chrono::duration_cast<std::chrono::milliseconds>(arEntry.GetTimeStamp().time_since_epoch()).count();
	jint error = arEntry.GetErrorCode();

	jobject le = pEnv->NewObject(mLogEntryClazz, mLogEntryInitMid, level, name, msg, time, error);
	assert(le != NULL);

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);
	jmethodID mid2 = pEnv->GetMethodID(clazz, "onLogEntry", "(Lcom/automatak/dnp3/LogEntry;)V");
	assert(mid2 != NULL);

	pEnv->CallVoidMethod(mProxy, mid2, le);
}
