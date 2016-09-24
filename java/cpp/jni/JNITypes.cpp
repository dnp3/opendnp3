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

#include "JNITypes.h"

namespace jcache
{
    bool init(JNIEnv* env)
    {
        auto success = true;

        success = jMasterStackConfig.init(env);
        if(!success) return false;
        success = jMasterConfig.init(env);
        if(!success) return false;
        success = jLinkLayerConfig.init(env);
        if(!success) return false;
        success = jLogEntry.init(env);
        if(!success) return false;
        success = jLogHandler.init(env);
        if(!success) return false;
        success = jClassField.init(env);
        if(!success) return false;
        success = jSOEHandler.init(env);
        if(!success) return false;
        success = jHeaderInfo.init(env);
        if(!success) return false;
        success = jGroupVariation.init(env);
        if(!success) return false;
        success = jQualifierCode.init(env);
        if(!success) return false;
        success = jTimestampMode.init(env);
        if(!success) return false;
        success = jIndexedValue.init(env);
        if(!success) return false;
        success = jBinaryInput.init(env);
        if(!success) return false;
        success = jDoubleBitBinaryInput.init(env);
        if(!success) return false;
        success = jAnalogInput.init(env);
        if(!success) return false;
        success = jCounter.init(env);
        if(!success) return false;
        success = jFrozenCounter.init(env);
        if(!success) return false;
        success = jBinaryOutputStatus.init(env);
        if(!success) return false;
        success = jAnalogOutputStatus.init(env);
        if(!success) return false;
        return true;
    }
}
