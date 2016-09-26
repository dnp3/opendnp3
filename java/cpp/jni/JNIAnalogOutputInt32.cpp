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

#include "JNIAnalogOutputInt32.h"

namespace jni
{
    bool AnalogOutputInt32::init(JNIEnv* env)
    {
        auto clazzTemp = env->FindClass("Lcom/automatak/dnp3/AnalogOutputInt32;");
        this->clazz = (jclass) env->NewGlobalRef(clazzTemp);
        env->DeleteLocalRef(clazzTemp);

        this->valueField = env->GetFieldID(this->clazz, "value", "I");
        if(!this->valueField) return false;

        this->statusField = env->GetFieldID(this->clazz, "status", "Lcom/automatak/dnp3/enums/CommandStatus;");
        if(!this->statusField) return false;

        return true;
    }

    void AnalogOutputInt32::cleanup(JNIEnv* env)
    {
        env->DeleteGlobalRef(this->clazz);
    }

    jint AnalogOutputInt32::getvalue(JNIEnv* env, jobject instance)
    {
        return env->GetIntField(instance, this->valueField);
    }

    jobject AnalogOutputInt32::getstatus(JNIEnv* env, jobject instance)
    {
        return env->GetObjectField(instance, this->statusField);
    }
}
