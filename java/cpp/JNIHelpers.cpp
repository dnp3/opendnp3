/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include "JNIHelpers.h"

#include <assert.h>


std::string JNIHelpers::GetString(jstring s, JNIEnv* pEnv)
{
	auto cstr = pEnv->GetStringUTFChars(s, NULL);
	std::string copy(cstr);
	pEnv->ReleaseStringUTFChars(s, cstr);
	return copy;
}

void JNIHelpers::AttachThread(JavaVM* apJVM)
{
	JNIEnv* pEnv;
	jint res = apJVM->AttachCurrentThread((void **)&pEnv, nullptr);
	assert(res == 0);
}

void JNIHelpers::DetachThread(JavaVM* apJVM)
{
	jint res = apJVM->DetachCurrentThread();
	assert(res == 0);
}

void JNIHelpers::DeleteGlobalReference(JavaVM* apJVM, jobject ref)
{
	JNIEnv* pEnv= nullptr;
	assert(apJVM->GetEnv((void **)&pEnv, JNI_VERSION_1_6) == 0);
	assert(pEnv != nullptr);
	pEnv->DeleteGlobalRef(ref);
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
	apJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != nullptr);
	return pEnv;	
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* apEnv, jclass clazz, const char* name, const char* sig)
{	
	jmethodID mid = apEnv->GetMethodID(clazz, name, sig);
	assert(mid != nullptr);
	return mid;
}

jmethodID JNIHelpers::GetMethodID(JNIEnv* apEnv, jobject obj, const char* name, const char* sig)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);
	return GetMethodID(apEnv, clazz, name, sig);
}

jint JNIHelpers::GetIntField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);
	jfieldID field = apEnv->GetFieldID(clazz, fieldId, "I");
	assert(field != nullptr);
	return apEnv->GetIntField(obj, field);
}

jlong JNIHelpers::GetLongField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);
	jfieldID field = apEnv->GetFieldID(clazz, fieldId, "J");
	assert(field != nullptr);
	return apEnv->GetLongField(obj, field);
}

bool JNIHelpers::GetBoolField(JNIEnv* apEnv, jobject obj, const char* fieldId)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);
	jfieldID field = apEnv->GetFieldID(clazz, fieldId, "Z");
	assert(field != nullptr);
	return apEnv->GetBooleanField(obj, field);
}

jobject JNIHelpers::GetObjectField(JNIEnv* apEnv, jobject obj, const char* fieldId, const char* fqcn)
{
	jclass clazz = apEnv->GetObjectClass(obj);
	assert(clazz != nullptr);
	jfieldID field = apEnv->GetFieldID(clazz, fieldId, fqcn);
	assert(field != nullptr);
	jobject ret = apEnv->GetObjectField(obj, field);	
	assert(ret != nullptr);
	return ret;
}





