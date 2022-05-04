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
#include "CallbackAdapters.h"

#include "Conversions.h"

#include <vcclr.h>

namespace Step Function I/O
{
    namespace DNP3
    {
        namespace Adapter
        {

            opendnp3::CommandResultCallbackT CallbackAdapters::Get(TaskCompletionSource<CommandTaskResult^>^ tcs)
            {
                gcroot<TaskCompletionSource<CommandTaskResult^>^> handle(tcs);

                return [handle](const opendnp3::ICommandTaskResult& res) -> void
                {
                    auto result = Conversions::ConvertCommandTaskResult(res);
                    handle->SetResult(result);
                };
            }

            opendnp3::RestartOperationCallbackT CallbackAdapters::Get(TaskCompletionSource<RestartResultType^>^ tcs)
            {
                gcroot<TaskCompletionSource<RestartResultType^>^> handle(tcs);

                return [handle](const opendnp3::RestartOperationResult& res) -> void
                {
                    auto result = gcnew RestartResultType((TaskCompletion)res.summary, Conversions::ConvertTimeDuration(res.restartTime));

                    handle->SetResult(result);
                };
            }

        }
    }
}
