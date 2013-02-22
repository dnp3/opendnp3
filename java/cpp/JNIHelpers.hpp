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

#ifndef __JNI_HELPERS_H_
#define __JNI_HELPERS_H_

#include <jni.h>
#include <string>

#include <functional>

#define MACRO_RETHROW_EXCEPTION(env, ex) \
		jclass exClass = env->FindClass("com/automatak/dnp3/DNP3ConfigException"); \
		env->ThrowNew(exClass, ex.GetErrorString().c_str());


class JNIHelpers
{
public:
	static std::string GetString(jstring, JNIEnv*);

	static void AttachThread(JavaVM* apJVM);
	static void DetachThread(JavaVM* apJVM);

	static void DeleteGlobalReference(JavaVM* apJVM, jobject ref);

	static JavaVM* GetJVMFromEnv(JNIEnv* apEnv);
	static JNIEnv* GetEnvFromJVM(JavaVM* apJVM);

	static jmethodID GetMethodID(JNIEnv* apEnv, jclass clazz, const char* name, const char* sig);
	static jmethodID GetMethodID(JNIEnv* apEnv, jobject obj, const char* name, const char* sig);
	static jclass GetClassForObject(JNIEnv* apEnv, jobject obj);

	static jint GetIntField(JNIEnv* apEnv, jobject obj, const char* fieldId);
	static jlong GetLongField(JNIEnv* apEnv, jobject obj, const char* fieldId);
	static bool GetBoolField(JNIEnv* apEnv, jobject obj, const char* fieldId);
	static jdouble GetDoubleField(JNIEnv* apEnv, jobject obj, const char* fieldId);
	static jobject GetObjectField(JNIEnv* apEnv, jobject obj, const char* fieldId, const char* fqcn);

	static void IterateOverListOfObjects(JNIEnv* apEnv, jobject list, std::function<void (jobject)> fun);
};

#endif
