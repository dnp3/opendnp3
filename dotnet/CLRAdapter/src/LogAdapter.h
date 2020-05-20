/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
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
#ifndef OPENDNP3CLR_LOG_ADAPTER_H
#define OPENDNP3CLR_LOG_ADAPTER_H

#include <opendnp3/logging/ILogHandler.h>

#include <memory>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class LogAdapter final : public opendnp3::ILogHandler
            {
            public:

                LogAdapter(Automatak::DNP3::Interface::ILogHandler^ proxy);

                static std::shared_ptr<opendnp3::ILogHandler> Create(Automatak::DNP3::Interface::ILogHandler ^ proxy);

                // logging error messages, etc
                virtual void log(opendnp3::ModuleId module,
                                 const char* id,
                                 opendnp3::LogLevel level,
                                 char const* location,
                                 char const* message) override;

            private:
                gcroot < Automatak::DNP3::Interface::ILogHandler^ > proxy;
            };

        }
    }
}

#endif
