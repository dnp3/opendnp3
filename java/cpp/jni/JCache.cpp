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

#include "JCache.h"

jni::MasterStackConfig JCache::masterStackConfig;
jni::MasterConfig JCache::masterConfig;
jni::LinkLayerConfig JCache::linkLayerConfig;
jni::LogEntry JCache::logEntry;
jni::LogHandler JCache::logHandler;
jni::ClassField JCache::classField;
jni::SOEHandler JCache::sOEHandler;
jni::HeaderInfo JCache::headerInfo;
jni::GroupVariation JCache::groupVariation;
jni::QualifierCode JCache::qualifierCode;
jni::TimestampMode JCache::timestampMode;
jni::IndexedValue JCache::indexedValue;
jni::BinaryInput JCache::binaryInput;
jni::DoubleBitBinaryInput JCache::doubleBitBinaryInput;
jni::AnalogInput JCache::analogInput;
jni::Counter JCache::counter;
jni::FrozenCounter JCache::frozenCounter;
jni::BinaryOutputStatus JCache::binaryOutputStatus;
jni::AnalogOutputStatus JCache::analogOutputStatus;

bool JCache::init(JNIEnv* env)
{
    auto success = true;

    success = masterStackConfig.init(env);
    if(!success) return false;
    success = masterConfig.init(env);
    if(!success) return false;
    success = linkLayerConfig.init(env);
    if(!success) return false;
    success = logEntry.init(env);
    if(!success) return false;
    success = logHandler.init(env);
    if(!success) return false;
    success = classField.init(env);
    if(!success) return false;
    success = sOEHandler.init(env);
    if(!success) return false;
    success = headerInfo.init(env);
    if(!success) return false;
    success = groupVariation.init(env);
    if(!success) return false;
    success = qualifierCode.init(env);
    if(!success) return false;
    success = timestampMode.init(env);
    if(!success) return false;
    success = indexedValue.init(env);
    if(!success) return false;
    success = binaryInput.init(env);
    if(!success) return false;
    success = doubleBitBinaryInput.init(env);
    if(!success) return false;
    success = analogInput.init(env);
    if(!success) return false;
    success = counter.init(env);
    if(!success) return false;
    success = frozenCounter.init(env);
    if(!success) return false;
    success = binaryOutputStatus.init(env);
    if(!success) return false;
    success = analogOutputStatus.init(env);
    if(!success) return false;
    return true;
}
