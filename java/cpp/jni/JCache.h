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

namespace jcache
{
    bool init(JNIEnv* env);

    static jni::MasterStackConfig jMasterStackConfig;
    static jni::MasterConfig jMasterConfig;
    static jni::LinkLayerConfig jLinkLayerConfig;
    static jni::LogEntry jLogEntry;
    static jni::LogHandler jLogHandler;
    static jni::ClassField jClassField;
    static jni::SOEHandler jSOEHandler;
    static jni::HeaderInfo jHeaderInfo;
    static jni::GroupVariation jGroupVariation;
    static jni::QualifierCode jQualifierCode;
    static jni::TimestampMode jTimestampMode;
    static jni::IndexedValue jIndexedValue;
    static jni::BinaryInput jBinaryInput;
    static jni::DoubleBitBinaryInput jDoubleBitBinaryInput;
    static jni::AnalogInput jAnalogInput;
    static jni::Counter jCounter;
    static jni::FrozenCounter jFrozenCounter;
    static jni::BinaryOutputStatus jBinaryOutputStatus;
    static jni::AnalogOutputStatus jAnalogOutputStatus;
}

#endif
