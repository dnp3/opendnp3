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
#ifndef OPENDNP3CLR_MASTER_CONVERSIONS_H
#define OPENDNP3CLR_MASTER_CONVERSIONS_H

#include "ICommandHeaderWorkAround.h"
#include "TaskCallbackAdapter.h"

#include <opendnp3/master/HeaderTypes.h>
#include <opendnp3/master/TaskConfig.h>
#include <opendnp3/master/TaskInfo.h>
#include <opendnp3/master/CommandSet.h>

#include <vector>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class MasterConversions
            {
            public:

                static std::vector<opendnp3::Header> ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers);

                static bool Convert(Header^ header, opendnp3::Header& output);

                static opendnp3::Header Convert(Header^ header);
                static opendnp3::Header ConvertCount8(CountHeader^ header);
                static opendnp3::Header ConvertCount16(CountHeader^ header);
                static opendnp3::Header ConvertRange8(RangeHeader^ header);
                static opendnp3::Header ConvertRange16(RangeHeader^ header);

                static opendnp3::TaskConfig Convert(TaskConfig^ config, ITaskCallback^ wrapper);
                static opendnp3::TaskConfig Convert(TaskConfig^ config);

                static opendnp3::CommandSet Convert(ICommandHeaders^ headers);

                static opendnp3::TaskId Convert(TaskId^ taskId);
                static TaskId^ Convert(const opendnp3::TaskId& id);

                static TaskInfo^ Convert(const opendnp3::TaskInfo& info);

                static std::shared_ptr<opendnp3::ITaskCallback> CreateTaskCallback(ITaskCallback^ callback);
            
            };

        }
    }
}

#endif
