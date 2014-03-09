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

#include <opendnp3/master/IMaster.h>

#include <opendnp3/outstation/IOutstation.h>
#include <opendnp3/outstation/ITimeWriteHandler.h>

#include <openpal/IExecutor.h>

#include <asiopal/UTCTimeSource.h>

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

	auto cleanup = [pJVM, global]()
	{
		JNIHelpers::DeleteGlobalReference(pJVM, global);
	};
	pChannel->AddDestructorHook(cleanup);

	pChannel->AddStateListener([pJVM, global](ChannelState state)
	{
		JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(pJVM);
		jmethodID changeID = JNIHelpers::GetMethodID(pEnv, global, "onStateChange", "(I)V");
		jint intstate = static_cast<jint>(state);
		pEnv->CallIntMethod(global, changeID, intstate);
	});
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jint logLevel, jobject publisher, jobject jconfig)
{

	auto pChannel = (IChannel*) ptr;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
	jobject global = pEnv->NewGlobalRef(publisher);
	auto pPublisher = new DataObserverAdapter(pJVM, global);
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	MasterStackConfig config = ConfigReader::ConvertMasterStackConfig(pEnv, jconfig);
	LogLevel lev = LogLevelFromType(logLevel);
	auto pMaster = pChannel->AddMaster(loggerId, lev, pPublisher, asiopal::UTCTimeSource::Inst(), config);
	auto pExecutor = pChannel->GetExecutor();
	auto cleanup = [pJVM, global, pPublisher]()
	{
		JNIHelpers::DeleteGlobalReference(pJVM, global);
		delete pPublisher;
	};

	pMaster->AddDestructorHook([pExecutor, cleanup]()
	{
		pExecutor->Post(cleanup);
	});

	pMaster->Enable(); // TODO - move this to bindings
	return (jlong) pMaster;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
(JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jint logLevel, jobject commandAdapter, jobject jconfig)
{
	auto pChannel = (IChannel*) ptr;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	SlaveStackConfig config = ConfigReader::ConvertSlaveStackConfig(pEnv, jconfig);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
	jobject global = pEnv->NewGlobalRef(commandAdapter);
	auto pCmdHandler = new CommandHandlerAdapter(pJVM, global);
	LogLevel lev = LogLevelFromType(logLevel);
	auto pOutstation = pChannel->AddOutstation(loggerId, lev, pCmdHandler, NullTimeWriteHandler::Inst(), config);  //TODO wrap time callbacks
	auto pExecutor = pChannel->GetExecutor();
	auto cleanup = [pJVM, global, pCmdHandler]()
	{
		JNIHelpers::DeleteGlobalReference(pJVM, global);
		delete pCmdHandler;
	};

	pOutstation->AddDestructorHook([pExecutor, cleanup]()
	{
		pExecutor->Post(cleanup);
	});

	pOutstation->Enable(); // TODO - move this to bindings
	return (jlong) pOutstation;
}

