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

#ifndef OPENDNP3JAVA_JNILINKLAYERCONFIG_H
#define OPENDNP3JAVA_JNILINKLAYERCONFIG_H

#include <jni.h>

namespace jni
{
    class LinkLayerConfig
    {
        public:

        bool init(JNIEnv* env);

        // field getter methods
        jboolean getisMaster(JNIEnv* env);
        jboolean getuseConfirms(JNIEnv* env);
        jint getnumRetry(JNIEnv* env);
        jint getlocalAddr(JNIEnv* env);
        jint getremoteAddr(JNIEnv* env);
        jobject getresponseTimeout(JNIEnv* env);
        jobject getkeepAliveTimeout(JNIEnv* env);

        private:

        jclass clazz = nullptr;

        // field ids
        jfieldID isMasterField = nullptr;
        jfieldID useConfirmsField = nullptr;
        jfieldID numRetryField = nullptr;
        jfieldID localAddrField = nullptr;
        jfieldID remoteAddrField = nullptr;
        jfieldID responseTimeoutField = nullptr;
        jfieldID keepAliveTimeoutField = nullptr;
    };
}

#endif
