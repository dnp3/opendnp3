/**
 * Copyright 2013-2016 Automatak, LLC
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

#include <asiodnp3/IChannel.h>

#include "adapters/ConfigReader.h"
#include "adapters/SOEHandlerAdapter.h"
#include "adapters/MasterApplicationAdapter.h"
#include "adapters/OutstationApplicationAdapter.h"
#include "adapters/CommandHandlerAdapter.h"

#include "adapters/CString.h"

#include <memory>

using namespace asiodnp3;
using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native, jboolean callshutdown)
{
	const auto channel = (std::shared_ptr<IChannel>*) native;

	if (callshutdown)
	{
		(*channel)->Shutdown();
	}
	
	delete channel; // drop our reference
}


JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv* env, jobject, jlong native, jstring jid, jobject handler, jobject application, jobject jconfig)
{
	const auto channel = (std::shared_ptr<IChannel>*) native;

	auto config = ConfigReader::ConvertMasterStackConfig(env, jconfig);
	auto soeAdapter = std::make_shared<SOEHandlerAdapter>(handler);
	auto appAdapter = std::make_shared<MasterApplicationAdapter>(application);

	CString id(env, jid);

	return (jlong) (*channel)->AddMaster(id.str(), soeAdapter, appAdapter, config);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1outstation
(JNIEnv* env, jobject, jlong native, jstring jid, jobject commandHandler, jobject application, jobject jconfig)
{
	const auto channel = (std::shared_ptr<IChannel>*) native;

	auto config = ConfigReader::ConvertOutstationStackConfig(env, jconfig);
	auto commandHandlerAdapter = std::make_shared<CommandHandlerAdapter>(application);
	auto applicationAdapter = std::make_shared<OutstationApplicationAdapter>(application);

	CString id(env, jid);

	return (jlong) (*channel)->AddOutstation(id.str(), commandHandlerAdapter, applicationAdapter, config);
}

