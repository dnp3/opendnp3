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

#include "SOEHandlerAdapter.h"

#include "JNI.h"
#include "JNIStrings.h"

using namespace opendnp3;

void SOEHandlerAdapter::Start()
{
	const auto env = JNI::GetEnv();

	if (!this->initialized)
	{
		using namespace classes::SOEHandler;

		this->startMethod = JNI::GetMethodIDFromObject(env, proxy, methods::start.name, methods::start.sig);
		this->endMethod = JNI::GetMethodIDFromObject(env, proxy, methods::end.name, methods::end.sig);

		// remaining mids

		this->initialized = true;
	}

	env->CallVoidMethod(proxy, this->startMethod);
}

void SOEHandlerAdapter::End()
{
	const auto env = JNI::GetEnv();
	env->CallVoidMethod(proxy, this->endMethod);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values)
{

}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values)
{

}

