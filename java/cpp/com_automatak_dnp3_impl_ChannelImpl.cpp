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
#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/IOutstation.h>

#include "JNIHelpers.h"
#include "DataObserverAdapter.h"
#include "CommandHandlerAdapter.h"

using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
  (JNIEnv *, jobject, jlong ptr)
{
	auto pChannel = (IChannel*) ptr;
	pChannel->Shutdown();
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject jlogLevel, jobject publisher)
{
	auto pChannel = (IChannel*) ptr;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
	jobject global = pEnv->NewGlobalRef(publisher);
	auto pPublisher = new DataObserverAdapter(pJVM, global);
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	auto pMaster = pChannel->AddMaster(loggerId, LEV_INFO, pPublisher, MasterStackConfig());
	pMaster->AddDestructorHook([pJVM, global]() { JNIHelpers::DeleteGlobalReference(pJVM, global); });
	pMaster->AddDestructorHook([pPublisher](){ delete pPublisher; });
	return (jlong) pMaster;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject jloglevel, jobject commandAdapter)
{
	auto pChannel = (IChannel*) ptr;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	SlaveStackConfig config;
	DeviceTemplate templ(3,3,3,3,3);
	config.device = templ;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);	
	jobject global = pEnv->NewGlobalRef(commandAdapter);
	auto pCmdHandler = new CommandHandlerAdapter(pJVM, global);	
	auto pOutstation = pChannel->AddOutstation(loggerId, LEV_INFO, pCmdHandler, config);
	pOutstation->AddDestructorHook([pJVM, global]() { JNIHelpers::DeleteGlobalReference(pJVM, global); });
	pOutstation->AddDestructorHook([pCmdHandler](){ delete pCmdHandler; });
	return (jlong) pOutstation;
}

