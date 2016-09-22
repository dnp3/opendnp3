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

#include "jni.h"

#include "JNIHelpers.h"

#include <assert.h>

using namespace std;

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{			
	JNI::Initialize(vm);
	return OPENDNP3_JNI_VERSION;
}

// initialize static objects
JavaVM * JNI::vm(nullptr);
LogHandlerCache JNI::logging;

jobject JNI::CreateGlobalRef(jobject ref)
{
	return GetEnv()->NewGlobalRef(ref);
}

void JNI::DeleteGlobalRef(jobject ref)
{
	GetEnv()->DeleteGlobalRef(ref);
}

bool JNI::AttachCurrentThread()
{
	JNIEnv* env;	
	return vm->AttachCurrentThread((void**)&env, nullptr) == 0;
}

bool JNI::DetachCurrentThread()
{
	return vm->DetachCurrentThread() == 0;
}

JNIEnv* JNI::GetEnv()
{
	JNIEnv* env = nullptr;
	jint ret = vm->GetEnv((void**)&env, OPENDNP3_JNI_VERSION);
	assert(ret == 0);
	assert(env != nullptr);
	return env;
}

void JNI::Initialize(JavaVM *vmin)
{
	assert(vmin);
	JNI::vm = vmin;		
}
