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
// Copyright 2016 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "JNIIPEndpoint.h"

namespace jni
{
    namespace cache
    {
        bool IPEndpoint::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/IPEndpoint;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->addressField = env->GetFieldID(this->clazz, "address", "Ljava/lang/String;");
            if(!this->addressField) return false;

            this->portField = env->GetFieldID(this->clazz, "port", "I");
            if(!this->portField) return false;

            return true;
        }

        void IPEndpoint::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        LocalRef<jstring> IPEndpoint::getaddress(JNIEnv* env, jobject instance)
        {
            return LocalRef<jstring>(env, (jstring) env->GetObjectField(instance, this->addressField));
        }

        jint IPEndpoint::getport(JNIEnv* env, jobject instance)
        {
            return env->GetIntField(instance, this->portField);
        }
    }
}
