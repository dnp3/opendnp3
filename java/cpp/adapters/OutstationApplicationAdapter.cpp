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

#include "OutstationApplicationAdapter.h"

#include "../jni/JCache.h"

using namespace jni;
using namespace opendnp3;

bool OutstationApplicationAdapter::SupportsWriteAbsoluteTime()
{
	const auto env = JNI::GetEnv();
	return !!JCache::OutstationApplication.supportsWriteAbsoluteTime(env, proxy);
}

bool OutstationApplicationAdapter::WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp)
{
	const auto env = JNI::GetEnv();
	return !!JCache::OutstationApplication.writeAbsoluteTime(env, proxy, timestamp.msSinceEpoch);
}

bool OutstationApplicationAdapter::SupportsAssignClass()
{
	const auto env = JNI::GetEnv();
	return !!JCache::OutstationApplication.supportsAssignClass(env, proxy);
}

void OutstationApplicationAdapter::RecordClassAssignment(AssignClassType type, PointClass clazz, uint16_t start, uint16_t stop)
{
	const auto env = JNI::GetEnv();

	auto jtype = JCache::AssignClassType.fromType(env, static_cast<jint>(type));
	auto jclazz = JCache::PointClass.fromType(env, static_cast<jint>(clazz));

	JCache::OutstationApplication.recordClassAssignment(env, proxy, jtype, jclazz, start, stop);
}

ApplicationIIN OutstationApplicationAdapter::GetApplicationIIN() const
{
	const auto env = JNI::GetEnv();
	auto jiin = JCache::OutstationApplication.getApplicationIIN(env, proxy);
	ApplicationIIN iin;
	iin.configCorrupt = !!JCache::ApplicationIIN.getconfigCorrupt(env, jiin);
	iin.deviceTrouble = !!JCache::ApplicationIIN.getdeviceTrouble(env, jiin);
	iin.localControl = !!JCache::ApplicationIIN.getlocalControl(env, jiin);
	iin.needTime = !!JCache::ApplicationIIN.getneedTime(env, jiin);
	return iin;
}


RestartMode OutstationApplicationAdapter::ColdRestartSupport() const
{
    const auto env = JNI::GetEnv();
	auto jmode = JCache::OutstationApplication.coldRestartSupport(env, proxy);
	uint8_t jmodeVal = static_cast<uint8_t>(JCache::RestartMode.toType(env, jmode));
	return RestartModeFromValue(jmodeVal);
}

RestartMode OutstationApplicationAdapter::WarmRestartSupport() const
{
    const auto env = JNI::GetEnv();
	auto jmode = JCache::OutstationApplication.coldRestartSupport(env, proxy);
	uint8_t jmodeVal = static_cast<uint8_t>(JCache::RestartMode.toType(env, jmode));
	return RestartModeFromValue(jmodeVal);
}


uint16_t OutstationApplicationAdapter::ColdRestart()
{
    const auto env = JNI::GetEnv();
    return JCache::OutstationApplication.coldRestart(env, proxy);
}

uint16_t OutstationApplicationAdapter::WarmRestart()
{
    const auto env = JNI::GetEnv();
    return JCache::OutstationApplication.warmRestart(env, proxy);
}

opendnp3::RestartMode OutstationApplicationAdapter::RestartModeFromValue(uint8_t arg)
{
	switch (arg) {
	case 1: return RestartMode::SUPPORTED_DELAY_FINE;
	case 2: return RestartMode::SUPPORTED_DELAY_COARSE;
	default: return RestartMode::UNSUPPORTED;
	}
}
