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
#ifndef OPENDNP3_JNI_HELPERS_H
#define OPENDNP3_JNI_HELPERS_H

#include <jni.h>
#include <string>
#include <functional>

#include <openpal/util/Uncopyable.h>

struct JNIHelpers : private openpal::StaticOnly
{

	static std::string GetString(jstring, JNIEnv*);

	static void AttachThread(JavaVM* jvm);
	static void DetachThread(JavaVM* jvm);

	static void DeleteGlobalReference(JavaVM* jvm, jobject ref);

	static JavaVM* GetJVMFromEnv(JNIEnv* env);
	static JNIEnv* GetEnvFromJVM(JavaVM* jvm);

	static jmethodID GetMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig);
	static jmethodID GetMethodID(JNIEnv* env, jobject obj, const char* name, const char* sig);
	static jclass GetClassForObject(JNIEnv* env, jobject obj);

	static jint GetIntField(JNIEnv* env, jobject obj, const char* fieldId);
	static jlong GetLongField(JNIEnv* env, jobject obj, const char* fieldId);
	static bool GetBoolField(JNIEnv* env, jobject obj, const char* fieldId);
	static jdouble GetDoubleField(JNIEnv* env, jobject obj, const char* fieldId);
	static jobject GetObjectField(JNIEnv* env, jobject obj, const char* fieldId, const char* fqcn);

	static void IterateOverListOfObjects(JNIEnv* env, jobject list, std::function<void (jobject)> fun);

};

#endif
