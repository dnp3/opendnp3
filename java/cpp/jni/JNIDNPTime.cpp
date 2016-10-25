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

#include "JNIDNPTime.h"

namespace jni
{
    namespace cache
    {
        bool DNPTime::init(JNIEnv* env)
        {
            auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/DNPTime;");
            this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
            env->DeleteLocalRef(clazzTemp);

            this->init0Constructor = env->GetMethodID(this->clazz, "<init>", "()V");
            if(!this->init0Constructor) return false;

            this->init1Constructor = env->GetMethodID(this->clazz, "<init>", "(J)V");
            if(!this->init1Constructor) return false;

            this->MAX_VALUEField = env->GetFieldID(this->clazz, "MAX_VALUE", "J");
            if(!this->MAX_VALUEField) return false;

            this->MIN_VALUEField = env->GetFieldID(this->clazz, "MIN_VALUE", "J");
            if(!this->MIN_VALUEField) return false;

            this->milliSecondSinceEpochField = env->GetFieldID(this->clazz, "milliSecondSinceEpoch", "J");
            if(!this->milliSecondSinceEpochField) return false;

            return true;
        }

        void DNPTime::cleanup(JNIEnv* env)
        {
            env->DeleteGlobalRef(this->clazz);
        }

        jobject DNPTime::init0(JNIEnv* env)
        {
            return env->NewObject(this->clazz, this->init0Constructor);
        }

        jobject DNPTime::init1(JNIEnv* env, jlong arg0)
        {
            return env->NewObject(this->clazz, this->init1Constructor, arg0);
        }

        jlong DNPTime::getMAX_VALUE(JNIEnv* env, jobject instance)
        {
            return env->GetLongField(instance, this->MAX_VALUEField);
        }

        jlong DNPTime::getMIN_VALUE(JNIEnv* env, jobject instance)
        {
            return env->GetLongField(instance, this->MIN_VALUEField);
        }

        jlong DNPTime::getmilliSecondSinceEpoch(JNIEnv* env, jobject instance)
        {
            return env->GetLongField(instance, this->milliSecondSinceEpochField);
        }
    }
}
