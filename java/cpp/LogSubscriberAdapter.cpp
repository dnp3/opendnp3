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
	mOnLogEntryCallbackMID(NULL)
{
	auto pEnv = JNIHelpers::GetEnvFromJVM(mpJVM);
	mOnLogEntryCallbackMID = JNIHelpers::GetMethodID(pEnv, aProxy, "onLogEntry", "(ILjava/lang/String;Ljava/lang/String;I)V");		
}

void LogSubscriberAdapter::Log(const opendnp3::LogEntry& arEntry)
{
	JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(mpJVM);

	jint level = arEntry.GetFilterLevel();
	jstring name = pEnv->NewStringUTF(arEntry.GetDeviceName().c_str());
	jstring msg = pEnv->NewStringUTF(arEntry.GetMessage().c_str());
	jint error = arEntry.GetErrorCode();
	
	pEnv->CallVoidMethod(mProxy, mOnLogEntryCallbackMID, level, name, msg, error);
}
