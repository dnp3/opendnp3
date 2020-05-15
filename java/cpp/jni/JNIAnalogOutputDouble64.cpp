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

#include "JNIAnalogOutputDouble64.h"

namespace jni
{
    namespace cache
    {
        bool AnalogOutputDouble64::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/AnalogOutputDouble64;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->constructor0 = env->GetMethodID(this->clazz, "<init>", "(DLcom/automatak/dnp3/enums/CommandStatus;)V");
            if(!this->constructor0) return false;

            this->valueField = env->GetFieldID(this->clazz, "value", "D");
            if(!this->valueField) return false;

            this->statusField = env->GetFieldID(this->clazz, "status", "Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->statusField) return false;

            return true;
        }

        void AnalogOutputDouble64::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JAnalogOutputDouble64> AnalogOutputDouble64::construct(JNIEnv* env, jdouble arg0, JCommandStatus arg1)
        {
            return LocalRef<JAnalogOutputDouble64>(env, JAnalogOutputDouble64(env->NewObject(this->clazz, this->constructor0, arg0, arg1)));
        }

        LocalRef<JCommandStatus> AnalogOutputDouble64::getstatus(JNIEnv* env, JAnalogOutputDouble64 instance)
        {
            return LocalRef<JCommandStatus>(env, env->GetObjectField(instance, this->statusField));
        }

        jdouble AnalogOutputDouble64::getvalue(JNIEnv* env, JAnalogOutputDouble64 instance)
        {
            return env->GetDoubleField(instance, this->valueField);
        }
    }
}
