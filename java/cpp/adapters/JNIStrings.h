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

#ifndef OPENDNP3JAVA_JNISTRINGS_H
#define OPENDNP3JAVA_JNISTRINGS_H

#include "MethodInfo.h"
#include "AdditionalJNIStrings.h"

namespace classes
{
    namespace MasterStackConfig
    {

        namespace fields
        {
            static const char* master = "master";
            static const char* link = "link";
        }
    }
}
namespace classes
{
    namespace MasterConfig
    {
        static const char* fqcn = "Lcom/automatak/dnp3/MasterConfig;";

        namespace fields
        {
            static const char* responseTimeout = "responseTimeout";
            static const char* timeSyncMode = "timeSyncMode";
            static const char* disableUnsolOnStartup = "disableUnsolOnStartup";
            static const char* ignoreRestartIIN = "ignoreRestartIIN";
            static const char* unsolClassMask = "unsolClassMask";
            static const char* startupIntegrityClassMask = "startupIntegrityClassMask";
            static const char* integrityOnEventOverflowIIN = "integrityOnEventOverflowIIN";
            static const char* eventScanOnEventsAvailableClassMask = "eventScanOnEventsAvailableClassMask";
            static const char* taskRetryPeriod = "taskRetryPeriod";
            static const char* taskStartTimeout = "taskStartTimeout";
            static const char* maxTxFragSize = "maxTxFragSize";
            static const char* maxRxFragSize = "maxRxFragSize";
        }
    }
}
namespace classes
{
    namespace LinkLayerConfig
    {
        static const char* fqcn = "Lcom/automatak/dnp3/LinkLayerConfig;";

        namespace fields
        {
            static const char* isMaster = "isMaster";
            static const char* useConfirms = "useConfirms";
            static const char* numRetry = "numRetry";
            static const char* localAddr = "localAddr";
            static const char* remoteAddr = "remoteAddr";
            static const char* responseTimeout = "responseTimeout";
            static const char* keepAliveTimeout = "keepAliveTimeout";
        }
    }
}
namespace classes
{
    namespace LogEntry
    {
        static const char* fqcn = "Lcom/automatak/dnp3/LogEntry;";

        namespace constructors
        {
            static const MethodInfo init0 = { "<init>", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" };
        }
    }
}
namespace classes
{
    namespace LogHandler
    {

        namespace methods
        {
            static const MethodInfo log = { "log", "(Lcom/automatak/dnp3/LogEntry;)V" };
        }
    }
}
namespace classes
{
    namespace TimeSyncMode
    {
        static const char* fqcn = "Lcom/automatak/dnp3/enums/TimeSyncMode;";
    }
}
namespace classes
{
    namespace Duration
    {
        static const char* fqcn = "Ljava/time/Duration;";
    }
}
namespace classes
{
    namespace ClassField
    {
        static const char* fqcn = "Lcom/automatak/dnp3/ClassField;";

        namespace fields
        {
            static const char* bitfield = "bitfield";
        }
    }
}
namespace classes
{
    namespace SOEHandler
    {

        namespace methods
        {
            static const MethodInfo end = { "end", "()V" };
            static const MethodInfo start = { "start", "()V" };
            static const MethodInfo processBI = { "processBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processDBI = { "processDBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processAI = { "processAI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processFC = { "processFC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processBOS = { "processBOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processC = { "processC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processAOS = { "processAOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
        }
    }
}
namespace classes
{
    namespace HeaderInfo
    {
        static const char* fqcn = "Lcom/automatak/dnp3/HeaderInfo;";

        namespace constructors
        {
            static const MethodInfo init0 = { "<init>", "(Lcom/automatak/dnp3/enums/GroupVariation;Lcom/automatak/dnp3/enums/QualifierCode;Lcom/automatak/dnp3/enums/TimestampMode;ZZI)V" };
        }
    }
}
namespace classes
{
    namespace GroupVariation
    {
        static const char* fqcn = "Lcom/automatak/dnp3/enums/GroupVariation;";

        namespace methods
        {
            static const MethodInfo values = { "values", "()[Lcom/automatak/dnp3/enums/GroupVariation;" };
            static const MethodInfo valueOf = { "valueOf", "(Ljava/lang/String;)Lcom/automatak/dnp3/enums/GroupVariation;" };
            static const MethodInfo toType = { "toType", "()I" };
            static const MethodInfo fromType = { "fromType", "(I)Lcom/automatak/dnp3/enums/GroupVariation;" };
        }
    }
}
namespace classes
{
    namespace QualifierCode
    {
        static const char* fqcn = "Lcom/automatak/dnp3/enums/QualifierCode;";

        namespace methods
        {
            static const MethodInfo values = { "values", "()[Lcom/automatak/dnp3/enums/QualifierCode;" };
            static const MethodInfo valueOf = { "valueOf", "(Ljava/lang/String;)Lcom/automatak/dnp3/enums/QualifierCode;" };
            static const MethodInfo toType = { "toType", "()I" };
            static const MethodInfo fromType = { "fromType", "(I)Lcom/automatak/dnp3/enums/QualifierCode;" };
        }
    }
}
namespace classes
{
    namespace TimestampMode
    {
        static const char* fqcn = "Lcom/automatak/dnp3/enums/TimestampMode;";

        namespace methods
        {
            static const MethodInfo values = { "values", "()[Lcom/automatak/dnp3/enums/TimestampMode;" };
            static const MethodInfo valueOf = { "valueOf", "(Ljava/lang/String;)Lcom/automatak/dnp3/enums/TimestampMode;" };
            static const MethodInfo toType = { "toType", "()I" };
        }
    }
}

#endif
