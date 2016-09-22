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

#ifndef OPENDNP3_JNI_H
#define OPENDNP3_JNI_H

#include <jni.h>
#include <string>
#include <openpal/util/Uncopyable.h>

extern "C" {
	jint JNI_OnLoad(JavaVM *vm, void *reserved);
}

#define OPENDNP3_JNI_VERSION JNI_VERSION_1_8  

class JNI : private openpal::StaticOnly
{

public:

	// called once during JNI_OnLoad
	static void Initialize(JavaVM *vm);

	static JNIEnv* GetEnv();

	static jobject CreateGlobalRef(jobject ref);
	static void DeleteGlobalRef(jobject ref);
	
	static bool AttachCurrentThread();
	static bool DetachCurrentThread();	

	// --- methods requiring an ENV ---

	static jclass FindClass(JNIEnv* env, const char* name);
	static jmethodID GetMethodIDFromClass(JNIEnv* env, jclass clazz, const char* name, const char* sig);
	static jmethodID GetMethodIDFromObject(JNIEnv* env, jobject obj, const char* name, const char* sig);
	static jclass GetClassForObject(JNIEnv* env, jobject obj);	
	

private:
	
	static JavaVM *vm;


};

#endif

