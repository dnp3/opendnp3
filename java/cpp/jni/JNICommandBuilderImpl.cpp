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

#include "JNICommandBuilderImpl.h"

namespace jni
{
    bool CommandBuilderImpl::init(JNIEnv* env)
    {
        auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/impl/CommandBuilderImpl;");
        this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
        env->DeleteLocalRef(clazzTemp);

        this->init0Constructor = env->GetMethodID(this->clazz, "<init>", "()V");
        if(!this->init0Constructor) return false;

        this->nativePointerField = env->GetFieldID(this->clazz, "nativePointer", "J");
        if(!this->nativePointerField) return false;

        return true;
    }

    void CommandBuilderImpl::cleanup(JNIEnv* env)
    {
        env->DeleteGlobalRef(this->clazz);
    }

    jobject CommandBuilderImpl::init0(JNIEnv* env)
    {
        return env->NewObject(this->clazz, this->init0Constructor);
    }

    jlong CommandBuilderImpl::getnativePointer(JNIEnv* env, jobject instance)
    {
        return env->GetLongField(instance, this->nativePointerField);
    }
}
