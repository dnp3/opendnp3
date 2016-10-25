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

#ifndef OPENDNP3JAVA_JNITIMEANDINTERVAL_H
#define OPENDNP3JAVA_JNITIMEANDINTERVAL_H

#include <jni.h>

namespace jni
{
    struct JCache;

    namespace cache
    {
        class TimeAndInterval
        {
            friend struct jni::JCache;

            bool init(JNIEnv* env);
            void cleanup(JNIEnv* env);

            public:

            // constructor methods
            jobject init3(JNIEnv* env, jobject arg0, jint arg1, jobject arg2);
            jobject init3(JNIEnv* env, jlong arg0, jint arg1, jobject arg2);

            // field getter methods
            jobject gettime(JNIEnv* env, jobject instance);
            jint getinterval(JNIEnv* env, jobject instance);
            jobject getunits(JNIEnv* env, jobject instance);

            private:

            jclass clazz = nullptr;

            // constructor method ids
            jmethodID init3Constructor = nullptr;
            jmethodID init3Constructor = nullptr;

            // field ids
        };
    }
}

#endif
