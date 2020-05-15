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

#include "JNIHeader.h"

namespace jni
{
    namespace cache
    {
        bool Header::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/Header;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->groupField = env->GetFieldID(this->clazz, "group", "B");
            if(!this->groupField) return false;

            this->variationField = env->GetFieldID(this->clazz, "variation", "B");
            if(!this->variationField) return false;

            this->qualifierField = env->GetFieldID(this->clazz, "qualifier", "Lcom/automatak/dnp3/enums/QualifierCode;");
            if(!this->qualifierField) return false;

            this->countField = env->GetFieldID(this->clazz, "count", "I");
            if(!this->countField) return false;

            this->startField = env->GetFieldID(this->clazz, "start", "I");
            if(!this->startField) return false;

            this->stopField = env->GetFieldID(this->clazz, "stop", "I");
            if(!this->stopField) return false;

            return true;
        }

        void Header::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jint Header::getcount(JNIEnv* env, JHeader instance)
        {
            return env->GetIntField(instance, this->countField);
        }

        jbyte Header::getgroup(JNIEnv* env, JHeader instance)
        {
            return env->GetByteField(instance, this->groupField);
        }

        LocalRef<JQualifierCode> Header::getqualifier(JNIEnv* env, JHeader instance)
        {
            return LocalRef<JQualifierCode>(env, env->GetObjectField(instance, this->qualifierField));
        }

        jint Header::getstart(JNIEnv* env, JHeader instance)
        {
            return env->GetIntField(instance, this->startField);
        }

        jint Header::getstop(JNIEnv* env, JHeader instance)
        {
            return env->GetIntField(instance, this->stopField);
        }

        jbyte Header::getvariation(JNIEnv* env, JHeader instance)
        {
            return env->GetByteField(instance, this->variationField);
        }
    }
}
