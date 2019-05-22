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
// Copyright 2013-2019 Automatak, LLC
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
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->endMethod = env->GetMethodID(this->clazz, "end", "()V");
            if(!this->endMethod) return false;

            this->processAIMethod = env->GetMethodID(this->clazz, "processAI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processAIMethod) return false;

            this->processAOSMethod = env->GetMethodID(this->clazz, "processAOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processAOSMethod) return false;

            this->processBIMethod = env->GetMethodID(this->clazz, "processBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processBIMethod) return false;

            this->processBOSMethod = env->GetMethodID(this->clazz, "processBOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processBOSMethod) return false;

            this->processCMethod = env->GetMethodID(this->clazz, "processC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processCMethod) return false;

            this->processDBIMethod = env->GetMethodID(this->clazz, "processDBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processDBIMethod) return false;

            this->processDNPTimeMethod = env->GetMethodID(this->clazz, "processDNPTime", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processDNPTimeMethod) return false;

            this->processFCMethod = env->GetMethodID(this->clazz, "processFC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V");
            if(!this->processFCMethod) return false;

            this->startMethod = env->GetMethodID(this->clazz, "start", "()V");
            if(!this->startMethod) return false;

            return true;
        }

        void SOEHandler::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void SOEHandler::end(JNIEnv* env, jobject instance)
        {
            env->CallVoidMethod(instance, this->endMethod);
        }

        void SOEHandler::processAI(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processAIMethod, arg0, arg1);
        }

        void SOEHandler::processAOS(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processAOSMethod, arg0, arg1);
        }

        void SOEHandler::processBI(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processBIMethod, arg0, arg1);
        }

        void SOEHandler::processBOS(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processBOSMethod, arg0, arg1);
        }

        void SOEHandler::processC(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processCMethod, arg0, arg1);
        }

        void SOEHandler::processDBI(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processDBIMethod, arg0, arg1);
        }

        void SOEHandler::processDNPTime(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processDNPTimeMethod, arg0, arg1);
        }

        void SOEHandler::processFC(JNIEnv* env, jobject instance, jobject arg0, jobject arg1)
        {
            env->CallVoidMethod(instance, this->processFCMethod, arg0, arg1);
        }

        void SOEHandler::start(JNIEnv* env, jobject instance)
        {
            env->CallVoidMethod(instance, this->startMethod);
        }
    }
}
