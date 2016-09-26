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

#ifndef OPENDNP3JAVA_JNIMASTERAPPLICATION_H
#define OPENDNP3JAVA_JNIMASTERAPPLICATION_H

#include <jni.h>

namespace jni
{
    class MasterApplication
    {
        friend struct JCache;

        bool init(JNIEnv* env);
        void cleanup(JNIEnv* env);

        public:

        // methods
        void onReceiveIIN(JNIEnv* env, jobject instance, jobject arg0);
        void onTaskComplete(JNIEnv* env, jobject instance, jobject arg0);
        void onTaskStart(JNIEnv* env, jobject instance, jobject arg0, jobject arg1);
        jlong getMillisecondsSinceEpoch(JNIEnv* env, jobject instance);
        jboolean assignClassDuringStartup(JNIEnv* env, jobject instance);
        jobject getClassAssignments(JNIEnv* env, jobject instance);

        private:

        jclass clazz = nullptr;

        // method ids
        jmethodID onReceiveIINMethod = nullptr;
        jmethodID onTaskCompleteMethod = nullptr;
        jmethodID onTaskStartMethod = nullptr;
        jmethodID getMillisecondsSinceEpochMethod = nullptr;
        jmethodID assignClassDuringStartupMethod = nullptr;
        jmethodID getClassAssignmentsMethod = nullptr;
    };
}

#endif
