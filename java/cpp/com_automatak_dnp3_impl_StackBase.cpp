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
#include "com_automatak_dnp3_impl_StackBase.h"

#include <opendnp3/IStack.h>
#include <openpal/IExecutor.h>

#include <iostream>

#include "JNIHelpers.hpp"

using namespace opendnp3;
using namespace openpal;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_StackBase_add_1native_1stack_1state_1listener
(JNIEnv* apEnv, jobject, jlong ptr, jobject jproxy)
{
	auto pStack = (IStack*) ptr;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	jobject global = apEnv->NewGlobalRef(jproxy);
	auto pExecutor = pStack->GetExecutor();
	pStack->AddDestructorHook([pJVM, global, pExecutor]()
	{
		pExecutor->Post([pJVM, global]()
		{
			JNIHelpers::DeleteGlobalReference(pJVM, global);
		});
	});

	pStack->AddStateListener([pJVM, pExecutor, global](StackState state)
	{
		JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(pJVM);
		jmethodID changeID = JNIHelpers::GetMethodID(pEnv, global, "onStackStateChange", "(I)V");
		int intstate = (int) state;
		pEnv->CallIntMethod(global, changeID, intstate);
	});
}

