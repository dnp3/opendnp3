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

#ifndef OPENDNP3JAVA_JNIMASTERCONFIG_H
#define OPENDNP3JAVA_JNIMASTERCONFIG_H

#include <jni.h>

namespace jni
{
    class MasterConfig
    {
        public:

        bool init(JNIEnv* env);

        // field getter methods
        jobject getresponseTimeout(JNIEnv* env);
        jobject gettimeSyncMode(JNIEnv* env);
        jboolean getdisableUnsolOnStartup(JNIEnv* env);
        jboolean getignoreRestartIIN(JNIEnv* env);
        jobject getunsolClassMask(JNIEnv* env);
        jobject getstartupIntegrityClassMask(JNIEnv* env);
        jboolean getintegrityOnEventOverflowIIN(JNIEnv* env);
        jobject geteventScanOnEventsAvailableClassMask(JNIEnv* env);
        jobject gettaskRetryPeriod(JNIEnv* env);
        jobject gettaskStartTimeout(JNIEnv* env);
        jint getmaxTxFragSize(JNIEnv* env);
        jint getmaxRxFragSize(JNIEnv* env);

        private:

        jclass clazz = nullptr;

        // field ids
        jfieldID responseTimeoutField = nullptr;
        jfieldID timeSyncModeField = nullptr;
        jfieldID disableUnsolOnStartupField = nullptr;
        jfieldID ignoreRestartIINField = nullptr;
        jfieldID unsolClassMaskField = nullptr;
        jfieldID startupIntegrityClassMaskField = nullptr;
        jfieldID integrityOnEventOverflowIINField = nullptr;
        jfieldID eventScanOnEventsAvailableClassMaskField = nullptr;
        jfieldID taskRetryPeriodField = nullptr;
        jfieldID taskStartTimeoutField = nullptr;
        jfieldID maxTxFragSizeField = nullptr;
        jfieldID maxRxFragSizeField = nullptr;
    };
}

#endif
