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
#ifndef OPENDNP3CLR_MASTER_OPERATIONS_ADAPTER_H
#define OPENDNP3CLR_MASTER_OPERATIONS_ADAPTER_H

#include "CallbackAdapters.h"
#include "MasterConversions.h"
#include "MasterScanAdapter.h"

#include <opendnp3/master/IMasterOperations.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private ref class MasterOperationsAdapter abstract
            {
            public:

                MasterOperationsAdapter(opendnp3::IMasterOperations* operations);

                virtual void SetLogFilters(LogFilter flags);
            
                virtual Task<TaskCompletion>^ Scan(IEnumerable<Header^>^ headers, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual Task<TaskCompletion>^ ScanAllObjects(System::Byte group, System::Byte variation, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual Task<TaskCompletion>^ ScanClasses(ClassField field, ISOEHandler^ soeHandler, TaskConfig^ config);
                
                virtual Task<TaskCompletion>^ ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual Task<TaskCompletion>^ Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config);

                virtual Task<RestartResultType^>^ Restart(RestartType restartType, TaskConfig^ config);

                virtual Task<TaskCompletion>^ PerformFunction(System::String^ name, FunctionCode func, IEnumerable<Header^>^ headers, TaskConfig^ config);

                virtual IMasterScan^ AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual IMasterScan^ AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual IMasterScan^ AddClassScan(ClassField field, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config);

                virtual IMasterScan^ AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config);												

                /// --- implement ICommandProcessor ----

                virtual Task<CommandTaskResult^>^ SelectAndOperate(ICommandHeaders^ headers, TaskConfig^ config);
                virtual Task<CommandTaskResult^>^ DirectOperate(ICommandHeaders^ headers, TaskConfig^ config);

                virtual Task<CommandTaskResult^>^ SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config);			
                virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config);				
                virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config);				
                virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config);				
                virtual Task<CommandTaskResult^>^ SelectAndOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config);
                
                virtual Task<CommandTaskResult^>^ DirectOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config);
                virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config);				
                virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config);
                virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config);
                virtual Task<CommandTaskResult^>^ DirectOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config);
                

            private:
                
                opendnp3::IMasterOperations* operations;
            };

        }
    }
}

#endif
