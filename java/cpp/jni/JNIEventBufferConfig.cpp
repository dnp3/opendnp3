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

#include "JNIEventBufferConfig.h"

namespace jni
{
    namespace cache
    {
        bool EventBufferConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/EventBufferConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->maxBinaryEventsField = env->GetFieldID(this->clazz, "maxBinaryEvents", "I");
            if(!this->maxBinaryEventsField) return false;

            this->maxDoubleBinaryEventsField = env->GetFieldID(this->clazz, "maxDoubleBinaryEvents", "I");
            if(!this->maxDoubleBinaryEventsField) return false;

            this->maxAnalogEventsField = env->GetFieldID(this->clazz, "maxAnalogEvents", "I");
            if(!this->maxAnalogEventsField) return false;

            this->maxCounterEventsField = env->GetFieldID(this->clazz, "maxCounterEvents", "I");
            if(!this->maxCounterEventsField) return false;

            this->maxFrozenCounterEventsField = env->GetFieldID(this->clazz, "maxFrozenCounterEvents", "I");
            if(!this->maxFrozenCounterEventsField) return false;

            this->maxBinaryOutputStatusEventsField = env->GetFieldID(this->clazz, "maxBinaryOutputStatusEvents", "I");
            if(!this->maxBinaryOutputStatusEventsField) return false;

            this->maxAnalogOutputStatusEventsField = env->GetFieldID(this->clazz, "maxAnalogOutputStatusEvents", "I");
            if(!this->maxAnalogOutputStatusEventsField) return false;

            return true;
        }

        void EventBufferConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jint EventBufferConfig::getmaxAnalogEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxAnalogEventsField);
        }

        jint EventBufferConfig::getmaxAnalogOutputStatusEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxAnalogOutputStatusEventsField);
        }

        jint EventBufferConfig::getmaxBinaryEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxBinaryEventsField);
        }

        jint EventBufferConfig::getmaxBinaryOutputStatusEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxBinaryOutputStatusEventsField);
        }

        jint EventBufferConfig::getmaxCounterEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxCounterEventsField);
        }

        jint EventBufferConfig::getmaxDoubleBinaryEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxDoubleBinaryEventsField);
        }

        jint EventBufferConfig::getmaxFrozenCounterEvents(JNIEnv* env, JEventBufferConfig instance)
        {
            return env->GetIntField(instance, this->maxFrozenCounterEventsField);
        }
    }
}
