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

#include "../jni/JCache.h"

using namespace opendnp3;

void SOEHandlerAdapter::Start()
{
		
	jni::JCache::SOEHandler.start(JNI::GetEnv(), proxy);	
}

void SOEHandlerAdapter::End()
{
	jni::JCache::SOEHandler.end(JNI::GetEnv(), proxy);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
	const auto env = JNI::GetEnv();

	auto jinfo = Convert(env, info);

	auto list = jni::JCache::ArrayList.init1(env, values.Count());

	auto add = [&](const Indexed<Binary>& meas) {
		auto jvalue = jni::JCache::BinaryInput.init3(env, meas.value.value, meas.value.flags.value, meas.value.time);
		auto jindexed = jni::JCache::IndexedValue.init2(env, jvalue, meas.index);
		jni::JCache::ArrayList.add(env, list, jindexed);
	};

	values.ForeachItem(add);
	
	jni::JCache::SOEHandler.processBI(env, proxy, jinfo, list);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{	
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	const auto env = JNI::GetEnv();
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	const auto env = JNI::GetEnv();
}

jobject SOEHandlerAdapter::Convert(JNIEnv* env, const opendnp3::HeaderInfo& info)
{	
	jint gvRaw = GroupVariationToType(info.gv);
	jint qcRaw = QualifierCodeToType(info.qualifier);
	jint tsModeRaw = static_cast<int>(info.tsmode);

	auto gv = jni::JCache::GroupVariation.fromType(env, GroupVariationToType(info.gv));
	auto qc = jni::JCache::QualifierCode.fromType(env, QualifierCodeToType(info.qualifier));
	auto tsmode = jni::JCache::TimestampMode.fromType(env, static_cast<jint>(info.tsmode));	
	jboolean isEvent = info.isEventVariation;
	jboolean flagsValid = info.flagsValid;
	jint headerIndex = info.headerIndex;

	return jni::JCache::HeaderInfo.init6(env, gv, qc, tsmode, isEvent, flagsValid, headerIndex);
}

