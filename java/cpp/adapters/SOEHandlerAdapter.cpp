/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SOEHandlerAdapter.h"

#include "../jni/JCache.h"
#include "../jni/JNIWrappers.h"

using namespace opendnp3;

void SOEHandlerAdapter::BeginFragment(const ResponseInfo& info)
{
    const auto env = JNI::GetEnv();	
    jni::JCache::SOEHandler.beginFragment(env, proxy, jni::JCache::ResponseInfo.construct(env, info.unsolicited, info.fir, info.fin));
}

void SOEHandlerAdapter::EndFragment(const ResponseInfo& info)
{
    const auto env = JNI::GetEnv();
    jni::JCache::SOEHandler.endFragment(env, proxy, jni::JCache::ResponseInfo.construct(env, info.unsolicited, info.fir, info.fin));
}

template<class T, class CreateMeas, class CallProxy>
void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info,
                                const opendnp3::ICollection<opendnp3::Indexed<T>>& values,
                                const CreateMeas& create_meas,
                                const CallProxy& call_proxy)
{
    const auto env = JNI::GetEnv();

    auto jinfo = Convert(env, info);

    auto jlist = jni::JCache::ArrayList.construct(env, static_cast<jint>(values.Count()));

    auto add = [&](const Indexed<T>& meas) {
        const auto jvalue = create_meas(env, meas.value);
        const auto jindexed = jni::JCache::IndexedValue.construct(env, jvalue.template as<jni::JObject>(), meas.index);
        jni::JCache::ArrayList.add(env, jlist, jindexed.template as<jni::JObject>());
    };

    values.ForeachItem(add);

    call_proxy(env, proxy, jinfo, jlist.as<jni::JIterable>());
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values)
{
    auto create = [](JNIEnv* env, const Binary& value) -> LocalRef<jni::JBinaryInput> {
        return jni::JCache::BinaryInput.construct(env, static_cast<jboolean>(value.value), jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };

    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processBI(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    auto create = [](JNIEnv* env, const DoubleBitBinary& value) -> LocalRef<jni::JDoubleBitBinaryInput> {
        auto evalue = jni::JCache::DoubleBit.fromType(env, DoubleBitSpec::to_type(value.value));
        return jni::JCache::DoubleBitBinaryInput.construct(env, evalue, jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processDBI(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values)
{
    auto create = [](JNIEnv* env, const Analog& value) -> LocalRef<jni::JAnalogInput> {
        return jni::JCache::AnalogInput.construct(env, value.value, jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processAI(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values)
{
    auto create = [](JNIEnv* env, const Counter& value) -> LocalRef<jni::JCounter> {
        return jni::JCache::Counter.construct(env, value.value, jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processC(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values)
{
    auto create = [](JNIEnv* env, const FrozenCounter& value) -> LocalRef<jni::JFrozenCounter> {
        return jni::JCache::FrozenCounter.construct(env, value.value, jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processFC(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    auto create = [](JNIEnv* env, const BinaryOutputStatus& value) -> LocalRef<jni::JBinaryOutputStatus> {
        return jni::JCache::BinaryOutputStatus.construct(env, static_cast<jboolean>(value.value), jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processBOS(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    auto create = [](JNIEnv* env, const AnalogOutputStatus& value) -> LocalRef<jni::JAnalogOutputStatus> {
        return jni::JCache::AnalogOutputStatus.construct(env, value.value, jni::JCache::Flags.construct(env, value.flags.value), Convert(env, value.time));
    };
    auto call = [](JNIEnv* env, jni::JSOEHandler proxy, jni::JHeaderInfo hinfo, jni::JIterable list) {
        jni::JCache::SOEHandler.processAOS(env, proxy, hinfo, list);
    };

    this->Process(info, values, create, call);
}

void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info,
                                const opendnp3::ICollection<opendnp3::DNPTime>& values)
{
    const auto env = JNI::GetEnv();

    auto jinfo = Convert(env, info);

    auto jlist = jni::JCache::ArrayList.construct(env, static_cast<jint>(values.Count()));

    auto add = [&](const DNPTime& value) {
        auto jvalue = jni::JCache::DNPTime.construct(env, value.value);
        jni::JCache::ArrayList.add(env, jlist, jvalue.as<jni::JObject>());
    };

    values.ForeachItem(add);

    jni::JCache::SOEHandler.processDNPTime(env, proxy, jinfo, jlist.as<jni::JIterable>());
}

LocalRef<jni::JHeaderInfo> SOEHandlerAdapter::Convert(JNIEnv* env, const opendnp3::HeaderInfo& info)
{
    auto gv = jni::JCache::GroupVariation.fromType(env, GroupVariationSpec::to_type(info.gv));
    auto qc = jni::JCache::QualifierCode.fromType(env, QualifierCodeSpec::to_type(info.qualifier));
    auto tsquality = jni::JCache::TimestampQuality.fromType(env, static_cast<jint>(info.tsquality));
    jboolean isEvent = static_cast<jboolean>(info.isEventVariation);
    jboolean flagsValid = static_cast<jboolean>(info.flagsValid);
    jint headerIndex = info.headerIndex;

    return jni::JCache::HeaderInfo.construct(env, gv, qc, tsquality, isEvent, flagsValid, headerIndex);
}

LocalRef<jni::JDNPTime> SOEHandlerAdapter::Convert(JNIEnv* env, const opendnp3::DNPTime& time)
{
    const auto jquality = jni::JCache::TimestampQuality.fromType(env, TimestampQualitySpec::to_type(time.quality));
    return jni::JCache::DNPTime.construct(env, time.value, jquality);
}
