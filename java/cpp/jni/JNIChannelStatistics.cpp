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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "JNIChannelStatistics.h"

namespace jni
{
    namespace cache
    {
        bool ChannelStatistics::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/ChannelStatistics;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->constructor0 = env->GetMethodID(this->clazz, "<init>", "(JJJJJJ)V");
            if(!this->constructor0) return false;

            return true;
        }

        void ChannelStatistics::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JChannelStatistics> ChannelStatistics::construct(JNIEnv* env, jlong arg0, jlong arg1, jlong arg2, jlong arg3, jlong arg4, jlong arg5)
        {
            return LocalRef<JChannelStatistics>(env, JChannelStatistics(env->NewObject(this->clazz, this->constructor0, arg0, arg1, arg2, arg3, arg4, arg5)));
        }
    }
}
