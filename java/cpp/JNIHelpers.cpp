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
#include "JNIHelpers.hpp"

#include <openpal/Location.h>

#include <assert.h>
#include <sstream>
#include <iostream>

using namespace std;


std::string JNIHelpers::GetString(jstring s, JNIEnv* pEnv)
{
	auto cstr = pEnv->GetStringUTFChars(s, nullptr);
	std::string copy(cstr);
	pEnv->ReleaseStringUTFChars(s, cstr);
	return copy;
}

void JNIHelpers::AttachThread(JavaVM* apJVM)
{
	JNIEnv* pEnv;
	jint res = apJVM->AttachCurrentThread((void**)&pEnv, nullptr);
	assert(res == 0);
}

void JNIHelpers::DetachThread(JavaVM* apJVM)
{
	jint res = apJVM->DetachCurrentThread();
	assert(res == 0);
}

void JNIHelpers::DeleteGlobalReference(JavaVM* apJVM, jobject ref)
{
	 GetEnvFromJVM(apJVM)->DeleteGlobalRef(ref);	
}


JavaVM* JNIHelpers::GetJVMFromEnv(JNIEnv* apEnv)
{
	JavaVM* pJVM = nullptr;
	apEnv->GetJavaVM(&pJVM);
	assert(pJVM != nullptr);
	return pJVM;
}

JNIEnv* JNIHelpers::GetEnvFromJVM(JavaVM* apJVM)
{
	JNIEnv* pEnv = nullptr;
	jint ret = apJVM->GetEnv((void**) &pEnv, JNI_VERSION_1_6);
	assert(ret == 0);
	assert(pEnv != nullptr);
	return pEnv;
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* apEnv, jclass clazz, const char* name, const char* sig)
{
	jmethodID mid = apEnv->GetMethodID(clazz, name, sig);
	assert(mid != nullptr);
	return mid;
}

jclass JNIHelpers::GetClassForObject(JNIEnv* apEnv, jobject obj)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);	
	return clazz;
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* apEnv, jobject obj, const char* name, const char* sig)
{
	return GetMethodID(apEnv, GetClassForObject(apEnv, obj), name, sig);
}

jint JNIHelpers::GetIntField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jfieldID field = apEnv->GetFieldID(GetClassForObject(apEnv, obj), fieldId, "I");
	assert(field != nullptr);	
	return apEnv->GetIntField(obj, field);
}

jlong JNIHelpers::GetLongField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jfieldID field = apEnv->GetFieldID(GetClassForObject(apEnv, obj), fieldId, "J");
	assert(field != nullptr);	
	return apEnv->GetLongField(obj, field);
}

bool JNIHelpers::GetBoolField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jfieldID field = apEnv->GetFieldID(GetClassForObject(apEnv, obj), fieldId, "Z");
	assert(field != nullptr);	
	return apEnv->GetBooleanField(obj, field);
}

jdouble JNIHelpers::GetDoubleField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jfieldID field = apEnv->GetFieldID(GetClassForObject(apEnv, obj), fieldId, "D");
	assert(field != nullptr);
	return apEnv->GetDoubleField(obj, field);
}

jobject JNIHelpers::GetObjectField(JNIEnv* apEnv, jobject obj, const char* fieldId, const char* fqcn)
{

	jfieldID field = apEnv->GetFieldID(GetClassForObject(apEnv, obj), fieldId, fqcn);
	assert(field != nullptr);	
	jobject ret = apEnv->GetObjectField(obj, field);
	assert(ret != nullptr);	
	return ret;
}

void JNIHelpers::IterateOverListOfObjects(JNIEnv* apEnv, jobject list, std::function<void (jobject)> fun)
{
	jmethodID sizeMID = JNIHelpers::GetMethodID(apEnv, list, "size", "()I");
	jint size = apEnv->CallIntMethod(list, sizeMID);

	jmethodID getMID = JNIHelpers::GetMethodID(apEnv, list, "get", "(I)Ljava/lang/Object;");

	for(jint i = 0; i < size; ++i) 
	{
		jobject obj = apEnv->CallObjectMethod(list, getMID, i);
		assert(obj != nullptr);		
		fun(obj);
	}
}




