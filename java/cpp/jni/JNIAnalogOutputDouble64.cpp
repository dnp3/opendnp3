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

#include "JNIAnalogOutputDouble64.h"

namespace jni
{
    bool AnalogOutputDouble64::init(JNIEnv* env)
    {
        auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/AnalogOutputDouble64;");
        this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
        env->DeleteLocalRef(clazzTemp);

        this->valueField = env->GetFieldID(this->clazz, "value", "D");
        if(!this->valueField) return false;

        this->statusField = env->GetFieldID(this->clazz, "status", "Lcom/automatak/dnp3/enums/CommandStatus;");
        if(!this->statusField) return false;

        return true;
    }

    void AnalogOutputDouble64::cleanup(JNIEnv* env)
    {
        env->DeleteGlobalRef(this->clazz);
    }

    jdouble AnalogOutputDouble64::getvalue(JNIEnv* env, jobject instance)
    {
        return env->GetDoubleField(instance, this->valueField);
    }

    jobject AnalogOutputDouble64::getstatus(JNIEnv* env, jobject instance)
    {
        return env->GetObjectField(instance, this->statusField);
    }
}
