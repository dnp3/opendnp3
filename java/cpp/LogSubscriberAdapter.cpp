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
#include <assert.h>

#include "JNIHelpers.hpp"

LogSubscriberAdapter::LogSubscriberAdapter(JavaVM* apJVM, jobject aProxy) :
	mpJVM(apJVM),
	mProxy(aProxy)
{

}

void LogSubscriberAdapter::Log(const openpal::LogEntry& arEntry)
{
	//get the timestamp early for better accuracy
	jlong time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void**) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);

	jclass leClz = pEnv->FindClass("com/automatak/dnp3/impl/LogEntryImpl");
	assert(leClz != NULL);
	jmethodID mid = pEnv->GetMethodID(leClz, "<init>", "(ILjava/lang/String;Ljava/lang/String;JI)V");
	assert(mid != NULL);

	jint level = arEntry.GetFilterLevel();
	jstring name = pEnv->NewStringUTF(arEntry.GetDeviceName().c_str());
	jstring msg = pEnv->NewStringUTF(arEntry.GetMessage().c_str());	
	jint error = arEntry.GetErrorCode();

	jobject le = pEnv->NewObject(leClz, mid, level, name, msg, time, error);

	assert(le != NULL);

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);
	jmethodID mid2 = pEnv->GetMethodID(clazz, "onLogEntry", "(Lcom/automatak/dnp3/LogEntry;)V");
	assert(mid2 != NULL);

	pEnv->CallVoidMethod(mProxy, mid2, le);
}
