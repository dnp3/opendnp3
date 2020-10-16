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

#include "JNICommandHandler.h"

namespace jni
{
    namespace cache
    {
        bool CommandHandler::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/CommandHandler;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "begin", "()V");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "end", "()V");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "operate", "(Lcom/automatak/dnp3/AnalogOutputInt16;ILcom/automatak/dnp3/Database;Lcom/automatak/dnp3/enums/OperateType;)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "operate", "(Lcom/automatak/dnp3/AnalogOutputInt32;ILcom/automatak/dnp3/Database;Lcom/automatak/dnp3/enums/OperateType;)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "operate", "(Lcom/automatak/dnp3/AnalogOutputFloat32;ILcom/automatak/dnp3/Database;Lcom/automatak/dnp3/enums/OperateType;)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "operate", "(Lcom/automatak/dnp3/ControlRelayOutputBlock;ILcom/automatak/dnp3/Database;Lcom/automatak/dnp3/enums/OperateType;)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "operate", "(Lcom/automatak/dnp3/AnalogOutputDouble64;ILcom/automatak/dnp3/Database;Lcom/automatak/dnp3/enums/OperateType;)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "select", "(Lcom/automatak/dnp3/AnalogOutputInt32;I)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method7) return false;

            this->method8 = env->GetMethodID(this->clazz, "select", "(Lcom/automatak/dnp3/AnalogOutputInt16;I)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method8) return false;

            this->method9 = env->GetMethodID(this->clazz, "select", "(Lcom/automatak/dnp3/AnalogOutputFloat32;I)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method9) return false;

            this->method10 = env->GetMethodID(this->clazz, "select", "(Lcom/automatak/dnp3/ControlRelayOutputBlock;I)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method10) return false;

            this->method11 = env->GetMethodID(this->clazz, "select", "(Lcom/automatak/dnp3/AnalogOutputDouble64;I)Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->method11) return false;

            return true;
        }

        void CommandHandler::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void CommandHandler::begin(JNIEnv* env, JCommandHandler instance)
        {
            env->CallVoidMethod(instance, this->method0);
        }

        void CommandHandler::end(JNIEnv* env, JCommandHandler instance)
        {
            env->CallVoidMethod(instance, this->method1);
        }

        LocalRef<JCommandStatus> CommandHandler::operate(JNIEnv* env, JCommandHandler instance, JAnalogOutputInt16 arg0, jint arg1, JDatabase arg2, JOperateType arg3)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method2, arg0, arg1, arg2, arg3));
        }

        LocalRef<JCommandStatus> CommandHandler::operate(JNIEnv* env, JCommandHandler instance, JAnalogOutputInt32 arg0, jint arg1, JDatabase arg2, JOperateType arg3)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method3, arg0, arg1, arg2, arg3));
        }

        LocalRef<JCommandStatus> CommandHandler::operate(JNIEnv* env, JCommandHandler instance, JAnalogOutputFloat32 arg0, jint arg1, JDatabase arg2, JOperateType arg3)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method4, arg0, arg1, arg2, arg3));
        }

        LocalRef<JCommandStatus> CommandHandler::operate(JNIEnv* env, JCommandHandler instance, JControlRelayOutputBlock arg0, jint arg1, JDatabase arg2, JOperateType arg3)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method5, arg0, arg1, arg2, arg3));
        }

        LocalRef<JCommandStatus> CommandHandler::operate(JNIEnv* env, JCommandHandler instance, JAnalogOutputDouble64 arg0, jint arg1, JDatabase arg2, JOperateType arg3)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method6, arg0, arg1, arg2, arg3));
        }

        LocalRef<JCommandStatus> CommandHandler::select(JNIEnv* env, JCommandHandler instance, JAnalogOutputInt32 arg0, jint arg1)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method7, arg0, arg1));
        }

        LocalRef<JCommandStatus> CommandHandler::select(JNIEnv* env, JCommandHandler instance, JAnalogOutputInt16 arg0, jint arg1)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method8, arg0, arg1));
        }

        LocalRef<JCommandStatus> CommandHandler::select(JNIEnv* env, JCommandHandler instance, JAnalogOutputFloat32 arg0, jint arg1)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method9, arg0, arg1));
        }

        LocalRef<JCommandStatus> CommandHandler::select(JNIEnv* env, JCommandHandler instance, JControlRelayOutputBlock arg0, jint arg1)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method10, arg0, arg1));
        }

        LocalRef<JCommandStatus> CommandHandler::select(JNIEnv* env, JCommandHandler instance, JAnalogOutputDouble64 arg0, jint arg1)
        {
            return LocalRef<JCommandStatus>(env, env->CallObjectMethod(instance, this->method11, arg0, arg1));
        }
    }
}
