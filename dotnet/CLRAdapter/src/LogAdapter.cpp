/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "LogAdapter.h"

#include "Conversions.h"

namespace Step Function I/O
{
    namespace DNP3
    {
        namespace Adapter
        {

            LogAdapter::LogAdapter(Step Function I/O::DNP3::Interface::ILogHandler^ proxy) : proxy(proxy)
            {}

            std::shared_ptr<opendnp3::ILogHandler> LogAdapter::Create(Step Function I/O::DNP3::Interface::ILogHandler ^ proxy)
            {
                return std::shared_ptr<opendnp3::ILogHandler>(new LogAdapter(proxy));
            }

            // logging error messages, etc
            void LogAdapter::log(opendnp3::ModuleId module,
                                 const char* id,
                                 opendnp3::LogLevel level,
                                 char const* location,
                                 char const* message)
            {
                proxy->Log(gcnew Step Function I/O::DNP3::Interface::LogEntry(level.value, Conversions::ConvertString(id),
                                                                      Conversions::ConvertString(location),
                                                                      Conversions::ConvertString(message)));
            }

        }
    }
}
