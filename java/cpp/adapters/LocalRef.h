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

#ifndef OPENDNP3JAVA_LOCALREF_H
#define OPENDNP3JAVA_LOCALREF_H

#include <assert.h>
#include <jni.h>

// RAII class for automatically disposing of local refs
template<class T> class LocalRef
{
    JNIEnv* const env;
    T ref;

    LocalRef(LocalRef&) = delete;

    LocalRef& operator=(LocalRef&) = delete;

public:
    LocalRef(JNIEnv* env, T ref) : env(env), ref(ref) {}

    ~LocalRef()
    {
        if (this->ref.value)
        {
            env->DeleteLocalRef(ref);
        }
    }

    LocalRef(LocalRef&& other) : env(other.env), ref(other.ref)
    {
        other.ref.value = nullptr;
    }

    T get() const
    {
        return ref;
    }

    operator T() const
    {
        return ref;
    }

    template <class U>
    U as() const
    {
        return U(ref.value);
    }
};

struct JString {

    JString(jstring value) : value(value) {}

    jstring value;

    operator jstring() const
    {
        return value;
    }
};


class LocalJString : public LocalRef<JString>
{   
public: 
    LocalJString(JNIEnv* env, const char* cstring) : 
        LocalRef<JString>(env, env->NewStringUTF(cstring)) {}
};

#endif
