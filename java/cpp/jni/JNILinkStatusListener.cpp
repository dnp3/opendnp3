//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "JNILinkStatusListener.h"

namespace jni
{
    namespace cache
    {
        bool LinkStatusListener::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/LinkStatusListener;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "onKeepAliveFailure", "()V");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "onKeepAliveInitiated", "()V");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "onKeepAliveSuccess", "()V");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "onStateChange", "(Lcom/automatak/dnp3/enums/LinkStatus;)V");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "onUnknownDestinationAddress", "(I)V");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "onUnknownSourceAddress", "(I)V");
            if(!this->method5) return false;

            return true;
        }

        void LinkStatusListener::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void LinkStatusListener::onKeepAliveFailure(JNIEnv* env, JLinkStatusListener instance)
        {
            env->CallVoidMethod(instance, this->method0);
        }

        void LinkStatusListener::onKeepAliveInitiated(JNIEnv* env, JLinkStatusListener instance)
        {
            env->CallVoidMethod(instance, this->method1);
        }

        void LinkStatusListener::onKeepAliveSuccess(JNIEnv* env, JLinkStatusListener instance)
        {
            env->CallVoidMethod(instance, this->method2);
        }

        void LinkStatusListener::onStateChange(JNIEnv* env, JLinkStatusListener instance, JLinkStatus arg0)
        {
            env->CallVoidMethod(instance, this->method3, arg0);
        }

        void LinkStatusListener::onUnknownDestinationAddress(JNIEnv* env, JLinkStatusListener instance, jint arg0)
        {
            env->CallVoidMethod(instance, this->method4, arg0);
        }

        void LinkStatusListener::onUnknownSourceAddress(JNIEnv* env, JLinkStatusListener instance, jint arg0)
        {
            env->CallVoidMethod(instance, this->method5, arg0);
        }
    }
}
