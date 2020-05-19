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
#ifndef OPENDNP3CLR_TASK_CALLBACK_ADAPTER_H
#define OPENDNP3CLR_TASK_CALLBACK_ADAPTER_H

#include "Conversions.h"

#include <opendnp3/util/Uncopyable.h>
#include <opendnp3/master/ITaskCallback.h>

#include <vcclr.h>
#include <memory>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            class TaskCallbackAdapter : public opendnp3::ITaskCallback, opendnp3::Uncopyable
            {
            public:

                static std::shared_ptr<opendnp3::ITaskCallback> Create(Automatak::DNP3::Interface::ITaskCallback^ proxy)
                {
                    return std::make_shared<TaskCallbackAdapter>(proxy);
                }

                TaskCallbackAdapter(Automatak::DNP3::Interface::ITaskCallback ^ proxy) : root(proxy) {}

                virtual void OnStart() sealed
                {
                    root->OnStart();
                }

                virtual void OnComplete(opendnp3::TaskCompletion result) sealed
                {
                    root->OnComplete((TaskCompletion) result);
                }

                virtual void OnDestroyed() sealed
                {
                    root->OnDestroyed();
                }

            private:

                gcroot < Automatak::DNP3::Interface::ITaskCallback^ > root;
            };
        
        }
    }
}

#endif
