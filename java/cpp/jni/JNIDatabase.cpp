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

#include "JNIDatabase.h"

namespace jni
{
    namespace cache
    {
        bool Database::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/Database;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->method0 = env->GetMethodID(this->clazz, "freezeCounter", "(IZLcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method0) return false;

            this->method1 = env->GetMethodID(this->clazz, "freezeCounter", "(IZ)V");
            if(!this->method1) return false;

            this->method2 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/AnalogInput;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method2) return false;

            this->method3 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/DoubleBitBinaryInput;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method3) return false;

            this->method4 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/BinaryInput;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method4) return false;

            this->method5 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/AnalogOutputStatus;I)V");
            if(!this->method5) return false;

            this->method6 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/Counter;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method6) return false;

            this->method7 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/BinaryOutputStatus;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method7) return false;

            this->method8 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/AnalogOutputStatus;ILcom/automatak/dnp3/enums/EventMode;)V");
            if(!this->method8) return false;

            this->method9 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/BinaryInput;I)V");
            if(!this->method9) return false;

            this->method10 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/DoubleBitBinaryInput;I)V");
            if(!this->method10) return false;

            this->method11 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/AnalogInput;I)V");
            if(!this->method11) return false;

            this->method12 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/BinaryOutputStatus;I)V");
            if(!this->method12) return false;

            this->method13 = env->GetMethodID(this->clazz, "update", "(Lcom/automatak/dnp3/Counter;I)V");
            if(!this->method13) return false;

            return true;
        }

        void Database::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        void Database::freezeCounter(JNIEnv* env, JDatabase instance, jint arg0, jboolean arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method0, arg0, arg1, arg2);
        }

        void Database::freezeCounter(JNIEnv* env, JDatabase instance, jint arg0, jboolean arg1)
        {
            env->CallVoidMethod(instance, this->method1, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JAnalogInput arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method2, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JDoubleBitBinaryInput arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method3, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JBinaryInput arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method4, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JAnalogOutputStatus arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method5, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JCounter arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method6, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JBinaryOutputStatus arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method7, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JAnalogOutputStatus arg0, jint arg1, JEventMode arg2)
        {
            env->CallVoidMethod(instance, this->method8, arg0, arg1, arg2);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JBinaryInput arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method9, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JDoubleBitBinaryInput arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method10, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JAnalogInput arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method11, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JBinaryOutputStatus arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method12, arg0, arg1);
        }

        void Database::update(JNIEnv* env, JDatabase instance, JCounter arg0, jint arg1)
        {
            env->CallVoidMethod(instance, this->method13, arg0, arg1);
        }
    }
}
