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

#include "JNIClassAssignment.h"

namespace jni
{
    namespace cache
    {
        bool ClassAssignment::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/ClassAssignment;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->groupField = env->GetFieldID(this->clazz, "group", "B");
            if(!this->groupField) return false;

            this->variationField = env->GetFieldID(this->clazz, "variation", "B");
            if(!this->variationField) return false;

            this->clazzField = env->GetFieldID(this->clazz, "clazz", "Lcom/automatak/dnp3/enums/PointClass;");
            if(!this->clazzField) return false;

            this->rangeField = env->GetFieldID(this->clazz, "range", "Lcom/automatak/dnp3/Range;");
            if(!this->rangeField) return false;

            return true;
        }

        void ClassAssignment::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JPointClass> ClassAssignment::getclazz(JNIEnv* env, JClassAssignment instance)
        {
            return LocalRef<JPointClass>(env, env->GetObjectField(instance, this->clazzField));
        }

        jbyte ClassAssignment::getgroup(JNIEnv* env, JClassAssignment instance)
        {
            return env->GetByteField(instance, this->groupField);
        }

        LocalRef<JRange> ClassAssignment::getrange(JNIEnv* env, JClassAssignment instance)
        {
            return LocalRef<JRange>(env, env->GetObjectField(instance, this->rangeField));
        }

        jbyte ClassAssignment::getvariation(JNIEnv* env, JClassAssignment instance)
        {
            return env->GetByteField(instance, this->variationField);
        }
    }
}
