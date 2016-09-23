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

#include "JNITypes.h"
#include "AdditionalJNIStrings.h"

namespace classes
{
    namespace MasterStackConfig
    {

        namespace fields
        {
            static const FieldId master = { "master" };
            static const FieldId link = { "link" };
        }
    }
}
namespace classes
{
    namespace MasterConfig
    {
        static const FQCN fqcn = { "Lcom/automatak/dnp3/MasterConfig;" };

        namespace fields
        {
            static const FieldId responseTimeout = { "responseTimeout" };
            static const FieldId timeSyncMode = { "timeSyncMode" };
            static const FieldId disableUnsolOnStartup = { "disableUnsolOnStartup" };
            static const FieldId ignoreRestartIIN = { "ignoreRestartIIN" };
            static const FieldId unsolClassMask = { "unsolClassMask" };
            static const FieldId startupIntegrityClassMask = { "startupIntegrityClassMask" };
            static const FieldId integrityOnEventOverflowIIN = { "integrityOnEventOverflowIIN" };
            static const FieldId eventScanOnEventsAvailableClassMask = { "eventScanOnEventsAvailableClassMask" };
            static const FieldId taskRetryPeriod = { "taskRetryPeriod" };
            static const FieldId taskStartTimeout = { "taskStartTimeout" };
            static const FieldId maxTxFragSize = { "maxTxFragSize" };
            static const FieldId maxRxFragSize = { "maxRxFragSize" };
        }
    }
}
namespace classes
{
    namespace LinkLayerConfig
    {
        static const FQCN fqcn = { "Lcom/automatak/dnp3/LinkLayerConfig;" };

        namespace fields
        {
            static const FieldId isMaster = { "isMaster" };
            static const FieldId useConfirms = { "useConfirms" };
            static const FieldId numRetry = { "numRetry" };
            static const FieldId localAddr = { "localAddr" };
            static const FieldId remoteAddr = { "remoteAddr" };
            static const FieldId responseTimeout = { "responseTimeout" };
            static const FieldId keepAliveTimeout = { "keepAliveTimeout" };
        }
    }
}
namespace classes
{
    namespace LogEntry
    {
        static const FQCN fqcn = { "Lcom/automatak/dnp3/LogEntry;" };

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
        static const FQCN fqcn = { "Lcom/automatak/dnp3/enums/TimeSyncMode;" };
    }
}
namespace classes
{
    namespace Duration
    {
        static const FQCN fqcn = { "Ljava/time/Duration;" };
    }
}
namespace classes
{
    namespace ClassField
    {
        static const FQCN fqcn = { "Lcom/automatak/dnp3/ClassField;" };

        namespace fields
        {
            static const FieldId bitfield = { "bitfield" };
        }
    }
}
namespace classes
{
    namespace SOEHandler
    {

        namespace methods
        {
            static const MethodInfo start = { "start", "()V" };
            static const MethodInfo end = { "end", "()V" };
            static const MethodInfo processFC = { "processFC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processBOS = { "processBOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processBI = { "processBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processDBI = { "processDBI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processAI = { "processAI", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processC = { "processC", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
            static const MethodInfo processAOS = { "processAOS", "(Lcom/automatak/dnp3/HeaderInfo;Ljava/lang/Iterable;)V" };
        }
    }
}
namespace classes
{
    namespace HeaderInfo
    {
        static const FQCN fqcn = { "Lcom/automatak/dnp3/HeaderInfo;" };

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
        static const FQCN fqcn = { "Lcom/automatak/dnp3/enums/GroupVariation;" };

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
        static const FQCN fqcn = { "Lcom/automatak/dnp3/enums/QualifierCode;" };

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
        static const FQCN fqcn = { "Lcom/automatak/dnp3/enums/TimestampMode;" };

        namespace methods
        {
            static const MethodInfo values = { "values", "()[Lcom/automatak/dnp3/enums/TimestampMode;" };
            static const MethodInfo valueOf = { "valueOf", "(Ljava/lang/String;)Lcom/automatak/dnp3/enums/TimestampMode;" };
            static const MethodInfo toType = { "toType", "()I" };
        }
    }
}

#endif
