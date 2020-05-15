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

#include "JNIMasterStackConfig.h"

namespace jni
{
    namespace cache
    {
        bool MasterStackConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/MasterStackConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->masterField = env->GetFieldID(this->clazz, "master", "Lcom/automatak/dnp3/MasterConfig;");
            if(!this->masterField) return false;

            this->linkField = env->GetFieldID(this->clazz, "link", "Lcom/automatak/dnp3/LinkLayerConfig;");
            if(!this->linkField) return false;

            return true;
        }

        void MasterStackConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<JLinkLayerConfig> MasterStackConfig::getlink(JNIEnv* env, JMasterStackConfig instance)
        {
            return LocalRef<JLinkLayerConfig>(env, env->GetObjectField(instance, this->linkField));
        }

        LocalRef<JMasterConfig> MasterStackConfig::getmaster(JNIEnv* env, JMasterStackConfig instance)
        {
            return LocalRef<JMasterConfig>(env, env->GetObjectField(instance, this->masterField));
        }
    }
}
