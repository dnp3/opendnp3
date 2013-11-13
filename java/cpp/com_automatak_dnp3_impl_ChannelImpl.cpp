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
#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/IOutstation.h>
#include <opendnp3/ITimeWriteHandler.h>

#include <openpal/Exception.h>

#include "JNIHelpers.hpp"
#include "DataObserverAdapter.hpp"
#include "CommandHandlerAdapter.hpp"
#include "ConfigReader.hpp"

#include <iostream>

using namespace opendnp3;
using namespace openpal;


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
(JNIEnv*, jobject, jlong ptr)
{
	auto pChannel = (IChannel*) ptr;
	pChannel->Shutdown();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_add_1native_1state_1change_1listener
(JNIEnv* apEnv, jobject, jlong ptr, jobject stateChangeProxy)
{
	auto pChannel = (IChannel*) ptr;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	jobject global = apEnv->NewGlobalRef(stateChangeProxy);
	pChannel->AddDestructorHook([pJVM, global]() {		
		JNIHelpers::DeleteGlobalReference(pJVM, global);		
	});
	pChannel->AddStateListener([pJVM, global](ChannelState state) {
		JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(pJVM);
		jmethodID changeID = JNIHelpers::GetMethodID(pEnv, global, "onStateChange", "(I)V");
		int intstate = state;
		pEnv->CallIntMethod(global, changeID, intstate);
	});
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jint logLevel, jobject publisher, jobject jconfig)
{
	try {
		auto pChannel = (IChannel*) ptr;
		JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
		jobject global = pEnv->NewGlobalRef(publisher);
		auto pPublisher = new DataObserverAdapter(pJVM, global);
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		MasterStackConfig config = ConfigReader::ConvertMasterStackConfig(pEnv, jconfig);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		auto pMaster = pChannel->AddMaster(loggerId, lev, pPublisher, config);
		pMaster->AddDestructorHook([pJVM, global]() {
			JNIHelpers::DeleteGlobalReference(pJVM, global);
		});
		pMaster->AddDestructorHook([pPublisher]() {
			delete pPublisher;
		});
		return (jlong) pMaster;
	}
	catch(const openpal::Exception& ex) {
		MACRO_RETHROW_EXCEPTION(pEnv, ex);
	}
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
(JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jint logLevel, jobject commandAdapter, jobject jconfig)
{
	try {
		auto pChannel = (IChannel*) ptr;
		std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
		SlaveStackConfig config = ConfigReader::ConvertSlaveStackConfig(pEnv, jconfig);
		JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
		jobject global = pEnv->NewGlobalRef(commandAdapter);
		auto pCmdHandler = new CommandHandlerAdapter(pJVM, global);
		FilterLevel lev = LogTypes::ConvertIntToFilterLevel(logLevel);
		auto pOutstation = pChannel->AddOutstation(loggerId, lev, pCmdHandler, NullTimeWriteHandler::Inst(), config);  //TODO wrap time callbacks
		pOutstation->AddDestructorHook([pJVM, global]() {
			JNIHelpers::DeleteGlobalReference(pJVM, global);
		});
		pOutstation->AddDestructorHook([pCmdHandler]() {
			delete pCmdHandler;
		});
		return (jlong) pOutstation;
	}
	catch(const openpal::Exception& ex) {
		MACRO_RETHROW_EXCEPTION(pEnv, ex);
	}
}

