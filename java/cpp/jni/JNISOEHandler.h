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

#ifndef OPENDNP3JAVA_JNISOEHANDLER_H
#define OPENDNP3JAVA_JNISOEHANDLER_H

#include <jni.h>

namespace jni
{
    class SOEHandler
    {
        public:

        bool init(JNIEnv* env);

        // methods
        void start(JNIEnv* env);
        void end(JNIEnv* env);
        void processAI(JNIEnv* env, jobject arg0, jobject arg1);
        void processFC(JNIEnv* env, jobject arg0, jobject arg1);
        void processAOS(JNIEnv* env, jobject arg0, jobject arg1);
        void processC(JNIEnv* env, jobject arg0, jobject arg1);
        void processDBI(JNIEnv* env, jobject arg0, jobject arg1);
        void processBOS(JNIEnv* env, jobject arg0, jobject arg1);
        void processBI(JNIEnv* env, jobject arg0, jobject arg1);

        private:

        jclass clazz = nullptr;

        // method ids
        jmethodID startMethod = nullptr;
        jmethodID endMethod = nullptr;
        jmethodID processAIMethod = nullptr;
        jmethodID processFCMethod = nullptr;
        jmethodID processAOSMethod = nullptr;
        jmethodID processCMethod = nullptr;
        jmethodID processDBIMethod = nullptr;
        jmethodID processBOSMethod = nullptr;
        jmethodID processBIMethod = nullptr;
    };
}

#endif
