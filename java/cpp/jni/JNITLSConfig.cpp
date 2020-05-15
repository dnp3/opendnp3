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

#include "JNITLSConfig.h"

namespace jni
{
    namespace cache
    {
        bool TLSConfig::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/TLSConfig;");
            if(!clazzTemp) return false;
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->peerCertFilePathField = env->GetFieldID(this->clazz, "peerCertFilePath", "Ljava/lang/String;");
            if(!this->peerCertFilePathField) return false;

            this->localCertFilePathField = env->GetFieldID(this->clazz, "localCertFilePath", "Ljava/lang/String;");
            if(!this->localCertFilePathField) return false;

            this->privateKeyFilePathField = env->GetFieldID(this->clazz, "privateKeyFilePath", "Ljava/lang/String;");
            if(!this->privateKeyFilePathField) return false;

            this->allowTLSv10Field = env->GetFieldID(this->clazz, "allowTLSv10", "Z");
            if(!this->allowTLSv10Field) return false;

            this->allowTLSv11Field = env->GetFieldID(this->clazz, "allowTLSv11", "Z");
            if(!this->allowTLSv11Field) return false;

            this->allowTLSv12Field = env->GetFieldID(this->clazz, "allowTLSv12", "Z");
            if(!this->allowTLSv12Field) return false;

            this->allowTLSv13Field = env->GetFieldID(this->clazz, "allowTLSv13", "Z");
            if(!this->allowTLSv13Field) return false;

            this->cipherListField = env->GetFieldID(this->clazz, "cipherList", "Ljava/lang/String;");
            if(!this->cipherListField) return false;

            return true;
        }

        void TLSConfig::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jboolean TLSConfig::getallowTLSv10(JNIEnv* env, JTLSConfig instance)
        {
            return env->GetBooleanField(instance, this->allowTLSv10Field);
        }

        jboolean TLSConfig::getallowTLSv11(JNIEnv* env, JTLSConfig instance)
        {
            return env->GetBooleanField(instance, this->allowTLSv11Field);
        }

        jboolean TLSConfig::getallowTLSv12(JNIEnv* env, JTLSConfig instance)
        {
            return env->GetBooleanField(instance, this->allowTLSv12Field);
        }

        jboolean TLSConfig::getallowTLSv13(JNIEnv* env, JTLSConfig instance)
        {
            return env->GetBooleanField(instance, this->allowTLSv13Field);
        }

        LocalRef<JString> TLSConfig::getcipherList(JNIEnv* env, JTLSConfig instance)
        {
            return LocalRef<JString>(env, (jstring) env->GetObjectField(instance, this->cipherListField));
        }

        LocalRef<JString> TLSConfig::getlocalCertFilePath(JNIEnv* env, JTLSConfig instance)
        {
            return LocalRef<JString>(env, (jstring) env->GetObjectField(instance, this->localCertFilePathField));
        }

        LocalRef<JString> TLSConfig::getpeerCertFilePath(JNIEnv* env, JTLSConfig instance)
        {
            return LocalRef<JString>(env, (jstring) env->GetObjectField(instance, this->peerCertFilePathField));
        }

        LocalRef<JString> TLSConfig::getprivateKeyFilePath(JNIEnv* env, JTLSConfig instance)
        {
            return LocalRef<JString>(env, (jstring) env->GetObjectField(instance, this->privateKeyFilePathField));
        }
    }
}
