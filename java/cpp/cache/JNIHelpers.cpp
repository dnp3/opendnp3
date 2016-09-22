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
#include "JNIHelpers.h"

#include <assert.h>
#include <sstream>
#include <iostream>

using namespace std;

std::string JNIHelpers::GetString(jstring s, JNIEnv* env)
{
	auto cstr = env->GetStringUTFChars(s, NULL);
	std::string copy(cstr);
	env->ReleaseStringUTFChars(s, cstr);
	return copy;
}

void JNIHelpers::AttachThread(JavaVM* jvm)
{
	JNIEnv* env;
	jint res = jvm->AttachCurrentThread((void**)&env, nullptr);
	assert(res == 0);
}

void JNIHelpers::DetachThread(JavaVM* jvm)
{
	jint res = jvm->DetachCurrentThread();
	assert(res == 0);
}

void JNIHelpers::DeleteGlobalReference(JavaVM* jvm, jobject ref)
{
	JNIEnv* env = GetEnvFromJVM(jvm);			
	env->DeleteGlobalRef(ref);
}


JavaVM* JNIHelpers::GetJVMFromEnv(JNIEnv* env)
{
	JavaVM* pJVM = nullptr;
	env->GetJavaVM(&pJVM);
	assert(pJVM != nullptr);
	return pJVM;
}

JNIEnv* JNIHelpers::GetEnvFromJVM(JavaVM* jvm)
{
	JNIEnv* env = nullptr;
	jint ret = jvm->GetEnv((void**) &env, JNI_VERSION_1_8);	
	assert(ret == 0);
	assert(env != nullptr);
	return env;
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig)
{
	jmethodID mid = env->GetMethodID(clazz, name, sig);
	assert(mid != nullptr);
	return mid;
}

jclass JNIHelpers::GetClassForObject(JNIEnv* env, jobject obj)
{
	jclass clazz = env->GetObjectClass(obj);
	assert(clazz != nullptr);
	return clazz;
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* env, jobject obj, const char* name, const char* sig)
{
	return GetMethodID(env, GetClassForObject(env, obj), name, sig);
}

jint JNIHelpers::GetIntField(JNIEnv* env, jobject obj, const char* fieldId)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), fieldId, "I");
	assert(field != nullptr);
	return env->GetIntField(obj, field);
}

jlong JNIHelpers::GetLongField(JNIEnv* env, jobject obj, const char* fieldId)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), fieldId, "J");
	assert(field != nullptr);
	return env->GetLongField(obj, field);
}

bool JNIHelpers::GetBoolField(JNIEnv* env, jobject obj, const char* fieldId)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), fieldId, "Z");
	assert(field != nullptr);
	return env->GetBooleanField(obj, field) != 0;
}

jdouble JNIHelpers::GetDoubleField(JNIEnv* env, jobject obj, const char* fieldId)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), fieldId, "D");
	assert(field != nullptr);
	return env->GetDoubleField(obj, field);
}

jobject JNIHelpers::GetObjectField(JNIEnv* env, jobject obj, const char* fieldId, const char* fqcn)
{

	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), fieldId, fqcn);
	assert(field != nullptr);
	jobject ret = env->GetObjectField(obj, field);
	assert(ret != nullptr);
	return ret;
}

void JNIHelpers::IterateOverListOfObjects(JNIEnv* env, jobject list, std::function<void (jobject)> fun)
{
	jmethodID sizeMID = JNIHelpers::GetMethodID(env, list, "size", "()I");
	jint size = env->CallIntMethod(list, sizeMID);

	jmethodID getMID = JNIHelpers::GetMethodID(env, list, "get", "(I)Ljava/lang/Object;");

	for(jint i = 0; i < size; ++i) {
		jobject obj = env->CallObjectMethod(list, getMID, i);
		assert(obj != nullptr);
		fun(obj);
	}
}




