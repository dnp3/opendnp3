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

#include "JNIOutstationApplication.h"

namespace jni
{
    namespace cache
    {
        bool OutstationApplication::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/OutstationApplication;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->coldRestartMethod = env->GetMethodID(this->clazz, "coldRestart", "()I");
            if(!this->coldRestartMethod) return false;

            this->coldRestartSupportMethod = env->GetMethodID(this->clazz, "coldRestartSupport", "()Lcom/automatak/dnp3/enums/RestartMode;");
            if(!this->coldRestartSupportMethod) return false;

            this->getApplicationIINMethod = env->GetMethodID(this->clazz, "getApplicationIIN", "()Lcom/automatak/dnp3/ApplicationIIN;");
            if(!this->getApplicationIINMethod) return false;

            this->recordClassAssignmentMethod = env->GetMethodID(this->clazz, "recordClassAssignment", "(Lcom/automatak/dnp3/enums/AssignClassType;Lcom/automatak/dnp3/enums/PointClass;II)V");
            if(!this->recordClassAssignmentMethod) return false;

            this->supportsAssignClassMethod = env->GetMethodID(this->clazz, "supportsAssignClass", "()Z");
            if(!this->supportsAssignClassMethod) return false;

            this->supportsWriteAbsoluteTimeMethod = env->GetMethodID(this->clazz, "supportsWriteAbsoluteTime", "()Z");
            if(!this->supportsWriteAbsoluteTimeMethod) return false;

            this->warmRestartMethod = env->GetMethodID(this->clazz, "warmRestart", "()I");
            if(!this->warmRestartMethod) return false;

            this->warmRestartSupportMethod = env->GetMethodID(this->clazz, "warmRestartSupport", "()Lcom/automatak/dnp3/enums/RestartMode;");
            if(!this->warmRestartSupportMethod) return false;

            this->writeAbsoluteTimeMethod = env->GetMethodID(this->clazz, "writeAbsoluteTime", "(J)Z");
            if(!this->writeAbsoluteTimeMethod) return false;

            return true;
        }

        void OutstationApplication::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jint OutstationApplication::coldRestart(JNIEnv* env, jobject instance)
        {
            return env->CallIntMethod(instance, this->coldRestartMethod);
        }

        LocalRef<jobject> OutstationApplication::coldRestartSupport(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->CallObjectMethod(instance, this->coldRestartSupportMethod));
        }

        LocalRef<jobject> OutstationApplication::getApplicationIIN(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->CallObjectMethod(instance, this->getApplicationIINMethod));
        }

        void OutstationApplication::recordClassAssignment(JNIEnv* env, jobject instance, jobject arg0, jobject arg1, jint arg2, jint arg3)
        {
            env->CallVoidMethod(instance, this->recordClassAssignmentMethod, arg0, arg1, arg2, arg3);
        }

        jboolean OutstationApplication::supportsAssignClass(JNIEnv* env, jobject instance)
        {
            return env->CallBooleanMethod(instance, this->supportsAssignClassMethod);
        }

        jboolean OutstationApplication::supportsWriteAbsoluteTime(JNIEnv* env, jobject instance)
        {
            return env->CallBooleanMethod(instance, this->supportsWriteAbsoluteTimeMethod);
        }

        jint OutstationApplication::warmRestart(JNIEnv* env, jobject instance)
        {
            return env->CallIntMethod(instance, this->warmRestartMethod);
        }

        LocalRef<jobject> OutstationApplication::warmRestartSupport(JNIEnv* env, jobject instance)
        {
            return LocalRef<jobject>(env, env->CallObjectMethod(instance, this->warmRestartSupportMethod));
        }

        jboolean OutstationApplication::writeAbsoluteTime(JNIEnv* env, jobject instance, jlong arg0)
        {
            return env->CallBooleanMethod(instance, this->writeAbsoluteTimeMethod, arg0);
        }
    }
}
