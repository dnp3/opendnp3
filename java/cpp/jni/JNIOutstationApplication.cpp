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

#include "JNIOutstationApplication.h"

namespace jni
{
    namespace cache
    {
        bool OutstationApplication::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/OutstationApplication;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "coldRestart", "()I");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "coldRestartSupport", "()Lcom/automatak/dnp3/enums/RestartMode;");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "getApplicationIIN", "()Lcom/automatak/dnp3/ApplicationIIN;");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "now", "()Lcom/automatak/dnp3/DNPTime;");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "onConfirmProcessed", "(ZJJJ)V");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "recordClassAssignment", "(Lcom/automatak/dnp3/enums/AssignClassType;Lcom/automatak/dnp3/enums/PointClass;II)V");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "supportsAssignClass", "()Z");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "supportsWriteAbsoluteTime", "()Z");
            if(!this->method7) return false;

            this->method8 = env->GetMethodID(this->clazz, "warmRestart", "()I");
            if(!this->method8) return false;

            this->method9 = env->GetMethodID(this->clazz, "warmRestartSupport", "()Lcom/automatak/dnp3/enums/RestartMode;");
            if(!this->method9) return false;

            this->method10 = env->GetMethodID(this->clazz, "writeAbsoluteTime", "(J)Z");
            if(!this->method10) return false;

            return true;
        }

        void OutstationApplication::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jint OutstationApplication::coldRestart(JNIEnv* env, JOutstationApplication instance)
        {
            return env->CallIntMethod(instance, this->method0);
        }

        LocalRef<JRestartMode> OutstationApplication::coldRestartSupport(JNIEnv* env, JOutstationApplication instance)
        {
            return LocalRef<JRestartMode>(env, env->CallObjectMethod(instance, this->method1));
        }

        LocalRef<JApplicationIIN> OutstationApplication::getApplicationIIN(JNIEnv* env, JOutstationApplication instance)
        {
            return LocalRef<JApplicationIIN>(env, env->CallObjectMethod(instance, this->method2));
        }

        LocalRef<JDNPTime> OutstationApplication::now(JNIEnv* env, JOutstationApplication instance)
        {
            return LocalRef<JDNPTime>(env, env->CallObjectMethod(instance, this->method3));
        }

        void OutstationApplication::onConfirmProcessed(JNIEnv* env, JOutstationApplication instance, jboolean arg0, jlong arg1, jlong arg2, jlong arg3)
        {
            env->CallVoidMethod(instance, this->method4, arg0, arg1, arg2, arg3);
        }

        void OutstationApplication::recordClassAssignment(JNIEnv* env, JOutstationApplication instance, JAssignClassType arg0, JPointClass arg1, jint arg2, jint arg3)
        {
            env->CallVoidMethod(instance, this->method5, arg0, arg1, arg2, arg3);
        }

        jboolean OutstationApplication::supportsAssignClass(JNIEnv* env, JOutstationApplication instance)
        {
            return env->CallBooleanMethod(instance, this->method6);
        }

        jboolean OutstationApplication::supportsWriteAbsoluteTime(JNIEnv* env, JOutstationApplication instance)
        {
            return env->CallBooleanMethod(instance, this->method7);
        }

        jint OutstationApplication::warmRestart(JNIEnv* env, JOutstationApplication instance)
        {
            return env->CallIntMethod(instance, this->method8);
        }

        LocalRef<JRestartMode> OutstationApplication::warmRestartSupport(JNIEnv* env, JOutstationApplication instance)
        {
            return LocalRef<JRestartMode>(env, env->CallObjectMethod(instance, this->method9));
        }

        jboolean OutstationApplication::writeAbsoluteTime(JNIEnv* env, JOutstationApplication instance, jlong arg0)
        {
            return env->CallBooleanMethod(instance, this->method10, arg0);
        }
    }
}
