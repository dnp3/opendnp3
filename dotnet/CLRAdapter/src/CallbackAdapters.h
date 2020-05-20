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
#ifndef OPENDNP3CLR_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_CALLBACK_ADAPTER_H

#include <opendnp3/util/StaticOnly.h>

#include <opendnp3/master/CommandResultCallbackT.h>
#include <opendnp3/master/RestartOperationResult.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;
using namespace System::Threading::Tasks;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            class CallbackAdapters : private opendnp3::StaticOnly
            {
            public:

                static opendnp3::CommandResultCallbackT Get(TaskCompletionSource<CommandTaskResult ^> ^ tcs);
                static opendnp3::RestartOperationCallbackT Get(TaskCompletionSource<RestartResultType^>^ tcs);
            };

        }
    }
}

#endif
