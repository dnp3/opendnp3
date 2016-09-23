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

struct Method { const char* name; const char* sig; };

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
            static const char* sig0 = "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V";
        }
    }
}
namespace classes
{
    namespace LogHandler
    {

        namespace methods
        {
            static const Method log = { "log", "(Lcom/automatak/dnp3/LogEntry;)V" };
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

#endif
