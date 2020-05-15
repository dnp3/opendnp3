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

#include "JNIAnalogOutputInt16.h"

namespace jni
{
    namespace cache
    {
        bool AnalogOutputInt16::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/AnalogOutputInt16;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->constructor0 = env->GetMethodID(this->clazz, "<init>", "(SLcom/automatak/dnp3/enums/CommandStatus;)V");
            if(!this->constructor0) return false;

            this->valueField = env->GetFieldID(this->clazz, "value", "S");
            if(!this->valueField) return false;

            this->statusField = env->GetFieldID(this->clazz, "status", "Lcom/automatak/dnp3/enums/CommandStatus;");
            if(!this->statusField) return false;

            return true;
        }

        void AnalogOutputInt16::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JAnalogOutputInt16> AnalogOutputInt16::construct(JNIEnv* env, jshort arg0, JCommandStatus arg1)
        {
            return LocalRef<JAnalogOutputInt16>(env, JAnalogOutputInt16(env->NewObject(this->clazz, this->constructor0, arg0, arg1)));
        }

        LocalRef<JCommandStatus> AnalogOutputInt16::getstatus(JNIEnv* env, JAnalogOutputInt16 instance)
        {
            return LocalRef<JCommandStatus>(env, env->GetObjectField(instance, this->statusField));
        }

        jshort AnalogOutputInt16::getvalue(JNIEnv* env, JAnalogOutputInt16 instance)
        {
            return env->GetShortField(instance, this->valueField);
        }
    }
}
