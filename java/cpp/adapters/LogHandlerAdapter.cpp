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

#include "JNI.h"
#include "JNIStrings.h"

void LogHandlerAdapter::Log(const openpal::LogEntry& entry)
{		
	using namespace classes::LogEntry;
	using namespace classes::LogHandler;

	const auto env = JNI::GetEnv();

	// cache these items
	if (!initialized)
	{
		this->logEntryClass = JNI::FindClass(env, classes::LogEntry::fqcn);
		this->logEntryConstructor = JNI::GetMethodIDFromClass(env, this->logEntryClass, "<init>", constructors::sig0);
		this->logMethod = JNI::GetMethodIDFromObject(env, proxy, methods::log.name, methods::log.sig);
		this->initialized = true;
	}

	const jint level = entry.GetFilters().GetBitfield();
	const jstring id = env->NewStringUTF(entry.GetAlias());
	const jstring location = env->NewStringUTF(entry.GetLocation());
	const jstring msg = env->NewStringUTF(entry.GetMessage());

	auto jentry = env->NewObject(this->logEntryClass, this->logEntryConstructor, level, id, location, msg);

	assert(jentry != nullptr);

	env->CallVoidMethod(proxy, logMethod, jentry);
}
