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

#include "JNICommandProcessor.h"

namespace jni
{
    namespace cache
    {
        bool CommandProcessor::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/CommandProcessor;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "directOperate", "(Lcom/automatak/dnp3/CommandHeaders;)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "directOperateAODouble64", "(Lcom/automatak/dnp3/AnalogOutputDouble64;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "directOperateAOFloat32", "(Lcom/automatak/dnp3/AnalogOutputFloat32;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "directOperateAOInt16", "(Lcom/automatak/dnp3/AnalogOutputInt16;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "directOperateAOInt32", "(Lcom/automatak/dnp3/AnalogOutputInt32;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "directOperateCROB", "(Lcom/automatak/dnp3/ControlRelayOutputBlock;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "selectAndOperate", "(Lcom/automatak/dnp3/CommandHeaders;)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "selectAndOperateAODouble64", "(Lcom/automatak/dnp3/AnalogOutputDouble64;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method7) return false;

            this->method8 = env->GetMethodID(this->clazz, "selectAndOperateAOFloat32", "(Lcom/automatak/dnp3/AnalogOutputFloat32;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method8) return false;

            this->method9 = env->GetMethodID(this->clazz, "selectAndOperateAOInt16", "(Lcom/automatak/dnp3/AnalogOutputInt16;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method9) return false;

            this->method10 = env->GetMethodID(this->clazz, "selectAndOperateAOInt32", "(Lcom/automatak/dnp3/AnalogOutputInt32;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method10) return false;

            this->method11 = env->GetMethodID(this->clazz, "selectAndOperateCROB", "(Lcom/automatak/dnp3/ControlRelayOutputBlock;I)Ljava/util/concurrent/CompletableFuture;");
            if(!this->method11) return false;

            return true;
        }

        void CommandProcessor::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperate(JNIEnv* env, JCommandProcessor instance, JCommandHeaders arg0)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method0, arg0));
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperateAODouble64(JNIEnv* env, JCommandProcessor instance, JAnalogOutputDouble64 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method1, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperateAOFloat32(JNIEnv* env, JCommandProcessor instance, JAnalogOutputFloat32 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method2, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperateAOInt16(JNIEnv* env, JCommandProcessor instance, JAnalogOutputInt16 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method3, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperateAOInt32(JNIEnv* env, JCommandProcessor instance, JAnalogOutputInt32 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method4, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::directOperateCROB(JNIEnv* env, JCommandProcessor instance, JControlRelayOutputBlock arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method5, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperate(JNIEnv* env, JCommandProcessor instance, JCommandHeaders arg0)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method6, arg0));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperateAODouble64(JNIEnv* env, JCommandProcessor instance, JAnalogOutputDouble64 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method7, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperateAOFloat32(JNIEnv* env, JCommandProcessor instance, JAnalogOutputFloat32 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method8, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperateAOInt16(JNIEnv* env, JCommandProcessor instance, JAnalogOutputInt16 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method9, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperateAOInt32(JNIEnv* env, JCommandProcessor instance, JAnalogOutputInt32 arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method10, arg0, arg1));
        }

        LocalRef<JCompletableFuture> CommandProcessor::selectAndOperateCROB(JNIEnv* env, JCommandProcessor instance, JControlRelayOutputBlock arg0, jint arg1)
        {
            return LocalRef<JCompletableFuture>(env, env->CallObjectMethod(instance, this->method11, arg0, arg1));
        }
    }
}
