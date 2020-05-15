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
// Copyright 2013-2020 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
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

#include "JNISOEHandler.h"

namespace jni
{
    namespace cache
    {
        bool SOEHandler::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/SOEHandler;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "beginFragment", "(Lcom/automatak/dnp3/ResponseInfo;)V");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "endFragment", "(Lcom/automatak/dnp3/ResponseInfo;)V");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "processAI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "processAOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "processBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "processBOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "processC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "processDBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method7) return false;

            this->method8 = env->GetMethodID(this->clazz, "processDNPTime", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method8) return false;

            this->method9 = env->GetMethodID(this->clazz, "processFC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->method9) return false;

            return true;
        }

        void SOEHandler::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void SOEHandler::beginFragment(JNIEnv* env, JSOEHandler instance, JResponseInfo arg0)
        {
            env->CallVoidMethod(instance, this->method0, arg0);
        }

        void SOEHandler::endFragment(JNIEnv* env, JSOEHandler instance, JResponseInfo arg0)
        {
            env->CallVoidMethod(instance, this->method1, arg0);
        }

        void SOEHandler::processAI(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method2, arg0, arg1);
        }

        void SOEHandler::processAOS(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method3, arg0, arg1);
        }

        void SOEHandler::processBI(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method4, arg0, arg1);
        }

        void SOEHandler::processBOS(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method5, arg0, arg1);
        }

        void SOEHandler::processC(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method6, arg0, arg1);
        }

        void SOEHandler::processDBI(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method7, arg0, arg1);
        }

        void SOEHandler::processDNPTime(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method8, arg0, arg1);
        }

        void SOEHandler::processFC(JNIEnv* env, JSOEHandler instance, JHeaderInfo arg0, JIterable arg1)
        {
            env->CallVoidMethod(instance, this->method9, arg0, arg1);
        }
    }
}
