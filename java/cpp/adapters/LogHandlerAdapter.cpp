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

#include "LogHandlerAdapter.h"

#include <iostream>
#include <assert.h>

#include "../cache/JNIHelpers.h"
#include "../cache/ClassNames.h"

LogHandlerAdapter::LogHandlerAdapter(JavaVM* jvm, jobject proxy) : 
	jvm(jvm), 
	proxy(JNIHelpers::GetEnvFromJVM(jvm)->NewGlobalRef(proxy))
{
	
}

LogHandlerAdapter::~LogHandlerAdapter()
{
	JNIHelpers::GetEnvFromJVM(jvm)->DeleteGlobalRef(proxy);
}

void LogHandlerAdapter::Log(const openpal::LogEntry& entry)
{		
	const auto env = JNIHelpers::GetEnvFromJVM(this->jvm);

	const jclass logEntryClass = env->FindClass(names::log_entry);
	assert(logEntryClass != nullptr);																					
	const jmethodID logEntryConstructor = env->GetMethodID(logEntryClass, "<init>", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	assert(logEntryConstructor != nullptr);
	
	const jint level = entry.GetFilters().GetBitfield();
	const jstring id = env->NewStringUTF(entry.GetAlias());
	const jstring location = env->NewStringUTF(entry.GetLocation());
	const jstring msg = env->NewStringUTF(entry.GetMessage());
				
	const jobject jLogEntry = env->NewObject(logEntryClass, logEntryConstructor, level, id, location, msg);
	assert(jLogEntry != nullptr);

	const jclass logHandlerClass = env->GetObjectClass(proxy);
	assert(logHandlerClass != nullptr);

	const jmethodID onLogEntryId = env->GetMethodID(logHandlerClass, "log", "(Lcom/automatak/dnp3/LogEntry;)V");
	assert(onLogEntryId != nullptr);
	
	env->CallVoidMethod(proxy, onLogEntryId, jLogEntry);
}
