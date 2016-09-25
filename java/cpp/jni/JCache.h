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

#ifndef OPENDNP3JAVA_OPENDNP3_JNITYPES_H
#define OPENDNP3JAVA_OPENDNP3_JNITYPES_H

#include "openpal/util/Uncopyable.h"

#include "JNIMasterStackConfig.h"
#include "JNIMasterConfig.h"
#include "JNILinkLayerConfig.h"
#include "JNILogEntry.h"
#include "JNILogHandler.h"
#include "JNIClassField.h"
#include "JNISOEHandler.h"
#include "JNIHeaderInfo.h"
#include "JNIGroupVariation.h"
#include "JNIQualifierCode.h"
#include "JNITimestampMode.h"
#include "JNIIndexedValue.h"
#include "JNIBinaryInput.h"
#include "JNIDoubleBitBinaryInput.h"
#include "JNIAnalogInput.h"
#include "JNICounter.h"
#include "JNIFrozenCounter.h"
#include "JNIBinaryOutputStatus.h"
#include "JNIAnalogOutputStatus.h"
#include "JNIDuration.h"

namespace jni
{
    struct JCache: private openpal::StaticOnly
    {
        static bool init(JNIEnv* env);

        static jni::MasterStackConfig masterStackConfig;
        static jni::MasterConfig masterConfig;
        static jni::LinkLayerConfig linkLayerConfig;
        static jni::LogEntry logEntry;
        static jni::LogHandler logHandler;
        static jni::ClassField classField;
        static jni::SOEHandler sOEHandler;
        static jni::HeaderInfo headerInfo;
        static jni::GroupVariation groupVariation;
        static jni::QualifierCode qualifierCode;
        static jni::TimestampMode timestampMode;
        static jni::IndexedValue indexedValue;
        static jni::BinaryInput binaryInput;
        static jni::DoubleBitBinaryInput doubleBitBinaryInput;
        static jni::AnalogInput analogInput;
        static jni::Counter counter;
        static jni::FrozenCounter frozenCounter;
        static jni::BinaryOutputStatus binaryOutputStatus;
        static jni::AnalogOutputStatus analogOutputStatus;
        static jni::Duration duration;
    };
}

#endif
