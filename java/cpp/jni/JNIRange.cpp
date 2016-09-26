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

#include "JNIRange.h"

namespace jni
{
    bool Range::init(JNIEnv* env)
    {
        auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/Range;");
        this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
        env->DeleteLocalRef(clazzTemp);

        this->isDefinedMethod = env->GetMethodID(this->clazz, "isDefined", "()Z");
        if(!this->isDefinedMethod) return false;

        this->MIN_INDEXField = env->GetFieldID(this->clazz, "MIN_INDEX", "I");
        if(!this->MIN_INDEXField) return false;

        this->MAX_INDEXField = env->GetFieldID(this->clazz, "MAX_INDEX", "I");
        if(!this->MAX_INDEXField) return false;

        this->startField = env->GetFieldID(this->clazz, "start", "I");
        if(!this->startField) return false;

        this->stopField = env->GetFieldID(this->clazz, "stop", "I");
        if(!this->stopField) return false;

        return true;
    }

    void Range::cleanup(JNIEnv* env)
    {
        env->DeleteGlobalRef(this->clazz);
    }

    jboolean Range::isDefined(JNIEnv* env, jobject instance)
    {
        return env->CallBooleanMethod(instance, this->isDefinedMethod);
    }

    jint Range::getMIN_INDEX(JNIEnv* env, jobject instance)
    {
        return env->GetIntField(instance, this->MIN_INDEXField);
    }

    jint Range::getMAX_INDEX(JNIEnv* env, jobject instance)
    {
        return env->GetIntField(instance, this->MAX_INDEXField);
    }

    jint Range::getstart(JNIEnv* env, jobject instance)
    {
        return env->GetIntField(instance, this->startField);
    }

    jint Range::getstop(JNIEnv* env, jobject instance)
    {
        return env->GetIntField(instance, this->stopField);
    }
}
