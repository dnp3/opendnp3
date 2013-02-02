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
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//

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



