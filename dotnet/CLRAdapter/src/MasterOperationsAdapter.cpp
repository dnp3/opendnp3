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
#include "MasterOperationsAdapter.h"

#include "Conversions.h"
#include "SOEHandlerAdapter.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            MasterOperationsAdapter::MasterOperationsAdapter(opendnp3::IMasterOperations* operations) : operations(operations)
            {}
            
            void MasterOperationsAdapter::SetLogFilters(LogFilter flags)
            {
                operations->SetLogFilters(opendnp3::LogLevel(flags.Flags));
            }
            
            Task<TaskCompletion>^ MasterOperationsAdapter::Scan(IEnumerable<Header^>^ headers, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                operations->Scan(vec, soeAdapter, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }

            Task<TaskCompletion>^ MasterOperationsAdapter::ScanAllObjects(System::Byte group, System::Byte variation, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                operations->ScanAllObjects(opendnp3::GroupVariationID(group, variation), soeAdapter, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }

            Task<TaskCompletion>^ MasterOperationsAdapter::ScanClasses(ClassField field, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                operations->ScanClasses(Conversions::ConvertClassField(field), soeAdapter, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }
                
            Task<TaskCompletion>^ MasterOperationsAdapter::ScanRange(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                opendnp3::GroupVariationID gvid(group, variation);
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                operations->ScanRange(gvid, start, stop, soeAdapter, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }

            Task<TaskCompletion>^ MasterOperationsAdapter::Write(TimeAndInterval^ value, System::UInt16 index, TaskConfig^ config)
            {
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                operations->Write(Conversions::ConvertMeas(value), index, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }

            Task<RestartResultType^>^ MasterOperationsAdapter::Restart(RestartType restartType, TaskConfig^ config)
            {
                auto tcs = gcnew TaskCompletionSource<RestartResultType^>();

                operations->Restart((opendnp3::RestartType) restartType,  CallbackAdapters::Get(tcs), MasterConversions::Convert(config));

                return tcs->Task;
            }

            Task<TaskCompletion>^ MasterOperationsAdapter::PerformFunction(System::String^ name, FunctionCode func, IEnumerable<Header^>^ headers, TaskConfig^ config)
            {
                auto proxy = gcnew TaskCompletionProxy(config->callback);
                auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
                auto nativeName = Conversions::ConvertString(name);
                operations->PerformFunction(nativeName, (opendnp3::FunctionCode) func, vec, MasterConversions::Convert(config, proxy));
                return proxy->CompletionTask;
            }

            IMasterScan^ MasterOperationsAdapter::AddScan(IEnumerable<Header^>^ headers, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                auto vec = MasterConversions::ConvertToVectorOfHeaders(headers);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                auto scan = operations->AddScan(Conversions::ConvertTimespan(period), vec, soeAdapter, MasterConversions::Convert(config));
                return gcnew MasterScanAdapter(scan);
            }

            IMasterScan^ MasterOperationsAdapter::AddAllObjectsScan(System::Byte group, System::Byte variation, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                opendnp3::GroupVariationID gvid(group, variation);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                auto scan = operations->AddAllObjectsScan(gvid, Conversions::ConvertTimespan(period), soeAdapter, MasterConversions::Convert(config));
                return gcnew MasterScanAdapter(scan);
            }

            IMasterScan^ MasterOperationsAdapter::AddClassScan(ClassField field, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                auto scan = operations->AddClassScan(Conversions::ConvertClassField(field), Conversions::ConvertTimespan(period), soeAdapter, MasterConversions::Convert(config));
                return gcnew MasterScanAdapter(scan);
            }

            IMasterScan^ MasterOperationsAdapter::AddRangeScan(System::Byte group, System::Byte variation, System::UInt16 start, System::UInt16 stop, System::TimeSpan period, ISOEHandler^ soeHandler, TaskConfig^ config)
            {
                opendnp3::GroupVariationID gvid(group, variation);
                auto soeAdapter = std::make_shared<SOEHandlerAdapter>(soeHandler);
                auto scan = operations->AddRangeScan(gvid, start, stop, Conversions::ConvertTimespan(period), soeAdapter, MasterConversions::Convert(config));
                return gcnew MasterScanAdapter(scan);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(ICommandHeaders^ headers, TaskConfig^ config)
            {
                auto tcs = gcnew TaskCompletionSource<CommandTaskResult^>();
                operations->SelectAndOperate(MasterConversions::Convert(headers), CallbackAdapters::Get(tcs), MasterConversions::Convert(config));
                return tcs->Task;
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(ICommandHeaders^ headers, TaskConfig^ config)
            {
                auto tcs = gcnew TaskCompletionSource<CommandTaskResult^>();
                operations->DirectOperate(MasterConversions::Convert(headers), CallbackAdapters::Get(tcs), MasterConversions::Convert(config));
                return tcs->Task;
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->SelectAndOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->SelectAndOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->SelectAndOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->SelectAndOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::SelectAndOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->SelectAndOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(ControlRelayOutputBlock^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->DirectOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(AnalogOutputDouble64^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->DirectOperate(headers, config);
            }
                
            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(AnalogOutputInt16^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->DirectOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(AnalogOutputInt32^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->DirectOperate(headers, config);
            }

            Task<CommandTaskResult^>^ MasterOperationsAdapter::DirectOperate(AnalogOutputFloat32^ command, System::UInt16 index, TaskConfig^ config)
            {
                ICommandHeaders^ headers = CommandHeader::From(IndexedValue::From(command, index));
                return this->DirectOperate(headers, config);
            }

        }
    }
}
