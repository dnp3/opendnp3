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

#ifndef OPENDNP3_JNI_H
#define OPENDNP3_JNI_H

#include <opendnp3/util/StaticOnly.h>

#include "LocalRef.h"
#include "../jni/JNIWrappers.h"
#include "../jni/JCache.h"

#include <functional>

#define OPENDNP3_JNI_VERSION JNI_VERSION_1_8

class JNI : private opendnp3::StaticOnly
{

public:
    // called once during JNI_OnLoad
    static void Initialize(JavaVM* vm);

    static JNIEnv* GetEnv();

    static bool AttachCurrentThread();
    static bool DetachCurrentThread();

    static jobject CreateGlobalRef(jobject ref);
    static void DeleteGlobalRef(jobject ref);

    

    template<class T,  class U>
    static void IterateWithIndex(JNIEnv* env, jni::JIterable iterable, const U& callback)
    {
        const auto iterator = jni::JCache::Iterable.iterator(env, iterable);

        int i = 0;
        while (jni::JCache::Iterator.hasNext(env, iterator) != 0u)
        {
            const auto local_ref = jni::JCache::Iterator.next(env, iterator);
            callback(T(local_ref.get().value), i);
            ++i;
        }
    }

    template<class T, class U>
    static void Iterate(JNIEnv* env, jni::JIterable iterable, const U& callback)
    {
        const auto iterator = jni::JCache::Iterable.iterator(env, iterable);

        while (jni::JCache::Iterator.hasNext(env, iterator))
        {
            const auto local_ref = jni::JCache::Iterator.next(env, iterator);
            callback(T(local_ref.get().value));
        }
    }

private:
    static JavaVM* vm;
};

#endif
