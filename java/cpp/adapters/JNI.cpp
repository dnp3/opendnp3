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

#include <assert.h>

using namespace std;

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{			
	JNI::Initialize(vm);
	return OPENDNP3_JNI_VERSION;
}

// initialize static objects
JavaVM * JNI::vm(nullptr);


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
	assert(env);
	return env;
}

void JNI::Initialize(JavaVM *vmin)
{
	assert(vmin);
	JNI::vm = vmin;		
}


jclass JNI::FindClass(JNIEnv* env, const FQCN& fqcn)
{
	auto ret = env->FindClass(fqcn.value);
	assert(ret);
	return ret;
}

jmethodID JNI::GetStaticMethodID(JNIEnv* env, const FQCN& fqcn, const MethodInfo& minfo)
{
	auto clazz = FindClass(env, fqcn);
	auto mid = env->GetStaticMethodID(clazz, minfo.name, minfo.sig);
	assert(mid);
	return mid;
}

jmethodID JNI::GetMethodIDFromClass(JNIEnv* env, jclass clazz, const MethodInfo& minfo)
{
	jmethodID ret = env->GetMethodID(clazz, minfo.name, minfo.sig);
	assert(ret);
	return ret;
}

jmethodID JNI::GetMethodIDFromObject(JNIEnv* env, jobject obj, const MethodInfo& minfo)
{
	auto method = GetMethodIDFromClass(env, GetClassForObject(env, obj), minfo);
	assert(method);
	return method;
}

jclass JNI::GetClassForObject(JNIEnv* env, jobject obj)
{
	jclass clazz = env->GetObjectClass(obj);
	assert(clazz);
	return clazz;
}

jint JNI::GetIntField(JNIEnv* env, jobject obj, const FieldId& id)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), id.value, "I");
	assert(field);
	return env->GetIntField(obj, field);
}

jlong JNI::GetLongField(JNIEnv* env, jobject obj, const FieldId& id)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), id.value, "J");
	assert(field);
	return env->GetLongField(obj, field);
}

bool JNI::GetBoolField(JNIEnv* env, jobject obj, const FieldId& id)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), id.value, "Z");
	assert(field);
	return env->GetBooleanField(obj, field) != 0;
}

jdouble JNI::GetDoubleField(JNIEnv* env, jobject obj, const FieldId& id)
{
	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), id.value, "D");
	assert(field);
	return env->GetDoubleField(obj, field);
}

jobject JNI::GetObjectField(JNIEnv* env, jobject obj, const FieldId& id, const FQCN& fqcn)
{

	jfieldID field = env->GetFieldID(GetClassForObject(env, obj), id.value, fqcn.value);
	assert(field);
	jobject ret = env->GetObjectField(obj, field);
	assert(ret);
	return ret;
}