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

#include "JNIDuration.h"

#include <iostream>

namespace jni
{
    bool Duration::init(JNIEnv* env)
    {

        this->clazz = env->FindClass("Ljava/time/Duration;");
        if(!this->clazz) return false;

        this->toMillisMethod = env->GetMethodID(this->clazz, "toMillis", "()J");
        if(!this->toMillisMethod) return false;		

        return true;
    }

    jlong Duration::toMillis(JNIEnv* env, jobject instance)
    {		
        return env->CallLongMethod(instance, this->toMillisMethod);
    }
}
