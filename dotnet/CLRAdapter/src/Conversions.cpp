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
#include "Conversions.h"

#include <msclr/marshal_cppstd.h>

namespace Automatak
{
namespace DNP3
{
    namespace Adapter
    {

        std::string Conversions::ConvertString(System::String ^ s)
        {
            return msclr::interop::marshal_as<std::string>(s);
        }

        System::String ^ Conversions::ConvertString(const std::string& s)
        {
            return gcnew System::String(s.c_str());
        }

        ChannelState Conversions::ConvertChannelState(opendnp3::ChannelState aState)
        {
            return (ChannelState)aState;
        }

        LinkStatus Conversions::ConvertLinkStatus(opendnp3::LinkStatus aState)
        {
            return (LinkStatus)aState;
        }

        IChannelStatistics ^ Conversions::ConvertChannelStats(const opendnp3::LinkStatistics& stats)
        {
            ChannelStatistics ^ ret = gcnew ChannelStatistics();

            ret->numBytesRx = stats.channel.numBytesRx;
            ret->numBytesTx = stats.channel.numBytesTx;
            ret->numOpen = stats.channel.numOpen;
            ret->numOpenFail = stats.channel.numOpenFail;
            ret->numClose = stats.channel.numClose;
            ret->numLinkFrameRx = stats.parser.numLinkFrameRx;
            ret->numLinkFrameTx = stats.channel.numLinkFrameTx;
            ret->numBadLinkFrameRx = stats.parser.numBadFCB + stats.parser.numBadFCV + stats.parser.numBadFunctionCode
                + stats.parser.numBadLength;
            ret->numCrcError = stats.parser.numBodyCrcError + stats.parser.numHeaderCrcError;

            return ret;
        }

        IStackStatistics ^ Conversions::ConvertStackStats(const opendnp3::StackStatistics& statistics)
        {
            StackStatistics ^ ret = gcnew StackStatistics();

            ret->numTransportErrorRx = statistics.transport.rx.numTransportErrorRx;
            ret->numTransportRx = statistics.transport.rx.numTransportRx;
            ret->numTransportTx = statistics.transport.tx.numTransportTx;

            return ret;
        }

        CommandTaskResult ^ Conversions::ConvertCommandTaskResult(const opendnp3::ICommandTaskResult& response)
        {
            auto convert = [](const opendnp3::CommandPointResult& value) -> CommandPointResult ^ {
                return gcnew CommandPointResult(value.headerIndex, value.index, (CommandPointState)value.state,
                                                (CommandStatus)value.status);
            };

            auto adapter = CreateAdapter<opendnp3::CommandPointResult, CommandPointResult ^>(convert);

            response.Foreach(adapter);

            return gcnew CommandTaskResult((TaskCompletion)response.summary, adapter.GetValues());
        }

        CommandStatus Conversions::ConvertCommandStatus(opendnp3::CommandStatus status)
        {
            return (CommandStatus)status;
        }

        opendnp3::CommandStatus Conversions::ConvertCommandStatus(CommandStatus status)
        {
            return (opendnp3::CommandStatus)status;
        }

        ControlRelayOutputBlock ^ Conversions::ConvertCommand(const opendnp3::ControlRelayOutputBlock& bo)
        {
            return gcnew ControlRelayOutputBlock(
                (OperationType)opendnp3::OperationTypeSpec::to_type(bo.opType),
                (TripCloseCode)opendnp3::TripCloseCodeSpec::to_type(bo.tcc),
                bo.clear,
                bo.count,
                bo.onTimeMS,
                bo.offTimeMS
            );
        }

        opendnp3::ControlRelayOutputBlock Conversions::ConvertCommand(ControlRelayOutputBlock ^ bo)
        {
            return opendnp3::ControlRelayOutputBlock(
                opendnp3::OperationTypeSpec::from_type((uint8_t)bo->opType),
                opendnp3::TripCloseCodeSpec::from_type((uint8_t)bo->tcc),
                bo->clear,
                bo->count,
                bo->onTime,
                bo->offTime
            );
        }

        opendnp3::AnalogOutputInt32 Conversions::ConvertCommand(AnalogOutputInt32 ^ sp)
        {
            return opendnp3::AnalogOutputInt32(sp->value);
        }

        AnalogOutputInt32 ^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt32& sp)
        {
            return gcnew AnalogOutputInt32(sp.value);
        }

        opendnp3::AnalogOutputInt16 Conversions::ConvertCommand(AnalogOutputInt16 ^ sp)
        {
            return opendnp3::AnalogOutputInt16(sp->value);
        }

        AnalogOutputInt16 ^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt16& sp)
        {
            return gcnew AnalogOutputInt16(sp.value);
        }

        opendnp3::AnalogOutputFloat32 Conversions::ConvertCommand(AnalogOutputFloat32 ^ sp)
        {
            return opendnp3::AnalogOutputFloat32(sp->value);
        }

        AnalogOutputFloat32 ^ Conversions::ConvertCommand(const opendnp3::AnalogOutputFloat32& sp)
        {
            return gcnew AnalogOutputFloat32(sp.value);
        }

        opendnp3::AnalogOutputDouble64 Conversions::ConvertCommand(AnalogOutputDouble64 ^ sp)
        {
            return opendnp3::AnalogOutputDouble64(sp->value);
        }

        AnalogOutputDouble64 ^ Conversions::ConvertCommand(const opendnp3::AnalogOutputDouble64& sp)
        {
            return gcnew AnalogOutputDouble64(sp.value);
        }

        Binary ^ Conversions::ConvertMeas(opendnp3::Binary meas)
        {
            return gcnew Binary(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        DoubleBitBinary ^ Conversions::ConvertMeas(opendnp3::DoubleBitBinary meas)
        {
            return gcnew DoubleBitBinary(static_cast<DoubleBit>(meas.value), gcnew Flags(meas.flags.value),
                                         ConvertTime(meas.time));
        }

        Analog ^ Conversions::ConvertMeas(opendnp3::Analog meas)
        {
            return gcnew Analog(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        Counter ^ Conversions::ConvertMeas(opendnp3::Counter meas)
        {
            return gcnew Counter(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        FrozenCounter ^ Conversions::ConvertMeas(opendnp3::FrozenCounter meas)
        {
            return gcnew FrozenCounter(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        AnalogOutputStatus ^ Conversions::ConvertMeas(opendnp3::AnalogOutputStatus meas)
        {
            return gcnew AnalogOutputStatus(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        BinaryOutputStatus ^ Conversions::ConvertMeas(opendnp3::BinaryOutputStatus meas)
        {
            return gcnew BinaryOutputStatus(meas.value, gcnew Flags(meas.flags.value), ConvertTime(meas.time));
        }

        OctetString ^ Conversions::ConvertMeas(const opendnp3::OctetString& meas)
        {
            return gcnew OctetString(Conversions::Convert(meas.ToBuffer()));
        }

        TimeAndInterval ^ Conversions::ConvertMeas(const opendnp3::TimeAndInterval& meas)
        {
            return gcnew TimeAndInterval(meas.time.value, meas.interval, meas.units);
        }

        BinaryCommandEvent ^ Conversions::ConvertMeas(const opendnp3::BinaryCommandEvent& meas)
        {
            return gcnew BinaryCommandEvent(meas.value, ConvertCommandStatus(meas.status), ConvertTime(meas.time));
        }

        AnalogCommandEvent ^ Conversions::ConvertMeas(const opendnp3::AnalogCommandEvent& meas)
        {
            return gcnew AnalogCommandEvent(meas.value, ConvertCommandStatus(meas.status), ConvertTime(meas.time));
        }

        opendnp3::DNPTime Conversions::ConvertTime(DNPTime^ time)
        {
            return opendnp3::DNPTime(time->ToEpoch(), (opendnp3::TimestampQuality)time->Quality);
        }

        DNPTime^ Conversions::ConvertTime(opendnp3::DNPTime time)
        {
            return DNPTime::FromEpoch(time.value, (TimestampQuality)time.quality);
        }

        opendnp3::Binary Conversions::ConvertMeas(Binary ^ meas)
        {
            return opendnp3::Binary(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::DoubleBitBinary Conversions::ConvertMeas(DoubleBitBinary ^ meas)
        {
            return opendnp3::DoubleBitBinary((opendnp3::DoubleBit)meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::Analog Conversions::ConvertMeas(Analog ^ meas)
        {
            return opendnp3::Analog(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::Counter Conversions::ConvertMeas(Counter ^ meas)
        {
            return opendnp3::Counter(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::FrozenCounter Conversions::ConvertMeas(FrozenCounter ^ meas)
        {
            return opendnp3::FrozenCounter(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::BinaryOutputStatus Conversions::ConvertMeas(BinaryOutputStatus ^ meas)
        {
            return opendnp3::BinaryOutputStatus(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::AnalogOutputStatus Conversions::ConvertMeas(AnalogOutputStatus ^ meas)
        {
            return opendnp3::AnalogOutputStatus(meas->Value, opendnp3::Flags(meas->Quality->Value), ConvertTime(meas->Timestamp));
        }

        opendnp3::OctetString Conversions::ConvertMeas(OctetString ^ meas)
        {
            uint8_t bytes[255];
            const uint8_t length = meas->Bytes->Length > 255 ? 255 : static_cast<uint8_t>(meas->Bytes->Length);
            for (uint8_t i = 0; i < length; ++i)
            {
                bytes[i] = meas->Bytes[i];
            }

            opendnp3::OctetString converted(opendnp3::Buffer(bytes, length));

            return converted;
        }

        opendnp3::TimeAndInterval Conversions::ConvertMeas(TimeAndInterval ^ meas)
        {
            return opendnp3::TimeAndInterval(opendnp3::DNPTime(meas->time), meas->interval, meas->units);
        }

        opendnp3::BinaryCommandEvent Conversions::ConvertMeas(BinaryCommandEvent ^ meas)
        {
            return opendnp3::BinaryCommandEvent(meas->Value, ConvertCommandStatus(meas->Status), ConvertTime(meas->Timestamp));
        }

        opendnp3::AnalogCommandEvent Conversions::ConvertMeas(AnalogCommandEvent ^ meas)
        {
            return opendnp3::AnalogCommandEvent(meas->Value, ConvertCommandStatus(meas->Status), ConvertTime(meas->Timestamp));
        }

        LinkHeader ^ Conversions::Convert(const opendnp3::LinkHeaderFields& fields)
        {
            return gcnew LinkHeader((LinkFunction)fields.func, fields.isFromMaster, fields.fcb, fields.fcvdfc,
                                    fields.addresses.destination, fields.addresses.source);
        }

        opendnp3::IPEndpoint Conversions::Convert(IPEndpoint ^ endpoint)
        {
            return opendnp3::IPEndpoint(ConvertString(endpoint->address), endpoint->port);
        }

        X509Info ^ Conversions::Convert(const opendnp3::X509Info& info)
        {
            return gcnew X509Info(info.depth, Conversions::Convert(info.sha1thumbprint),
                                  Conversions::ConvertString(info.subjectName));
        }

        opendnp3::SerialSettings Conversions::ConvertSerialSettings(SerialSettings ^ settings)
        {
            opendnp3::SerialSettings s;
            s.deviceName = ConvertString(settings->port);
            s.baud = settings->baud;
            s.dataBits = settings->dataBits;
            s.stopBits = (opendnp3::StopBits)settings->stopBits;
            s.parity = (opendnp3::Parity)settings->parity;
            s.flowType = (opendnp3::FlowControl)settings->flowControl;
            return s;
        }

        opendnp3::TimeDuration Conversions::ConvertMilliseconds(System::Int64 ms)
        {
            return opendnp3::TimeDuration::Milliseconds(ms);
        }

        opendnp3::TimeDuration Conversions::ConvertTimespan(System::TimeSpan ts)
        {
            return ConvertMilliseconds(ts.Ticks / System::TimeSpan::TicksPerMillisecond);
        }

        System::TimeSpan Conversions::ConvertTimeDuration(const opendnp3::TimeDuration& duration)
        {
            return System::TimeSpan::FromMilliseconds(
                (double)std::chrono::duration_cast<std::chrono::milliseconds>(duration.value).count());
        }

        opendnp3::ClassField Conversions::ConvertClassField(ClassField classField)
        {
            return opendnp3::ClassField(classField.ClassMask);
        }

        opendnp3::TLSConfig Conversions::Convert(TLSConfig ^ config)
        {
            return opendnp3::TLSConfig(Conversions::ConvertString(config->peerCertFilePath),
                                       Conversions::ConvertString(config->localCertFilePath),
                                       Conversions::ConvertString(config->privateKeyFilePath),
                                       config->allowTLSv10,
                                       config->allowTLSv11,
                                       config->allowTLSv12,
                                       config->allowTLSv13,
                                       Conversions::ConvertString(config->cipherList));
        }

        opendnp3::LinkConfig Conversions::ConvertConfig(LinkConfig ^ config)
        {
            return opendnp3::LinkConfig(config->isMaster, config->localAddr, config->remoteAddr,
                                        ConvertTimespan(config->responseTimeout), ConvertTimespan(config->keepAliveTimeout));
        }

        opendnp3::EventBufferConfig Conversions::ConvertConfig(EventBufferConfig ^ cm)
        {
            opendnp3::EventBufferConfig config;
            config.maxBinaryEvents = cm->maxBinaryEvents;
            config.maxDoubleBinaryEvents = cm->maxDoubleBinaryEvents;
            config.maxCounterEvents = cm->maxCounterEvents;
            config.maxFrozenCounterEvents = cm->maxFrozenCounterEvents;
            config.maxAnalogEvents = cm->maxAnalogEvents;
            config.maxBinaryOutputStatusEvents = cm->maxBinaryOutputStatusEvents;
            config.maxAnalogOutputStatusEvents = cm->maxAnalogOutputStatusEvents;
            config.maxOctetStringEvents = cm->maxOctetStringEvents;
            return config;
        }

        opendnp3::OutstationConfig Conversions::ConvertConfig(OutstationConfig ^ config)
        {
            opendnp3::OutstationConfig oc;

            oc.params = ConvertConfig(config->config);
            oc.eventBufferConfig = ConvertConfig(config->buffer);

            return oc;
        }

        opendnp3::OutstationParams Conversions::ConvertConfig(OutstationParams ^ config)
        {
            opendnp3::OutstationParams params;

            params.allowUnsolicited = config->allowUnsolicited;
            params.typesAllowedInClass0 = opendnp3::StaticTypeBitField(config->typesAllowedInClass0.mask);
            params.maxControlsPerRequest = config->maxControlsPerRequest;
            params.maxTxFragSize = config->maxTxFragSize;
            params.maxRxFragSize = config->maxRxFragSize;
            params.selectTimeout = ConvertTimespan(config->selectTimeout);
            params.solConfirmTimeout = ConvertTimespan(config->solicitedConfirmTimeout);
            params.unsolClassMask = ConvertClassField(config->unsolClassMask);
            params.unsolConfirmTimeout = ConvertTimespan(config->unsolConfirmTimeout);
            params.numUnsolRetries = ConvertNumRetries(config->numUnsolRetries);
            params.respondToAnyMaster = config->respondToAnyMaster;
            params.noDefferedReadDuringUnsolicitedNullResponse = config->noDefferedReadDuringUnsolicitedNullResponse;

            return params;
        }

        opendnp3::MasterParams Conversions::ConvertConfig(MasterConfig ^ config)
        {
            opendnp3::MasterParams mp;

            mp.disableUnsolOnStartup = config->disableUnsolOnStartup;
            mp.integrityOnEventOverflowIIN = config->integrityOnEventOverflowIIN;
            mp.responseTimeout = ConvertTimespan(config->responseTimeout);
            mp.startupIntegrityClassMask = ConvertClassField(config->startupIntegrityClassMask);
            mp.eventScanOnEventsAvailableClassMask = ConvertClassField(config->eventScanOnEventsAvailableClassMask);
            mp.taskRetryPeriod = ConvertTimespan(config->taskRetryPeriod);
            mp.taskStartTimeout = ConvertTimespan(config->taskStartTimeout);
            mp.timeSyncMode = (opendnp3::TimeSyncMode)config->timeSyncMode;
            mp.unsolClassMask = ConvertClassField(config->unsolClassMask);
            mp.controlQualifierMode = (opendnp3::IndexQualifierMode)config->controlQualifierMode;

            return mp;
        }

        opendnp3::MasterStackConfig Conversions::ConvertConfig(MasterStackConfig ^ config)
        {
            opendnp3::MasterStackConfig cfg;
            cfg.master = ConvertConfig(config->master);
            cfg.link = ConvertConfig(config->link);
            return cfg;
        }

        opendnp3::OutstationStackConfig Conversions::ConvertConfig(OutstationStackConfig ^ config)
        {
            opendnp3::OutstationStackConfig cfg;

            cfg.database = Convert(config->databaseTemplate);
            cfg.link = ConvertConfig(config->link);
            cfg.outstation = ConvertConfig(config->outstation);

            return cfg;
        }

        opendnp3::NumRetries Conversions::ConvertNumRetries(NumRetries ^ numRetries)
        {
            if (numRetries->isInfinite)
            {
                return opendnp3::NumRetries::Infinite();
            }
            else
            {
                return opendnp3::NumRetries::Fixed(numRetries->maxNumRetries);
            }
        }

        opendnp3::DatabaseConfig Conversions::Convert(DatabaseTemplate ^ lhs)
        {
            opendnp3::DatabaseConfig config;

            config.binary_input = Conversions::ConvertConfigMap<opendnp3::BinaryConfig>(lhs->binary);
            config.double_binary = Conversions::ConvertConfigMap<opendnp3::DoubleBitBinaryConfig>(lhs->doubleBinary);
            config.analog_input = Conversions::ConvertConfigMap<opendnp3::AnalogConfig>(lhs->analog);
            config.counter = Conversions::ConvertConfigMap<opendnp3::CounterConfig>(lhs->counter);
            config.frozen_counter = Conversions::ConvertConfigMap<opendnp3::FrozenCounterConfig>(lhs->frozenCounter);
            config.binary_output_status = Conversions::ConvertConfigMap<opendnp3::BOStatusConfig>(lhs->binaryOutputStatus);
            config.analog_output_status = Conversions::ConvertConfigMap<opendnp3::AOStatusConfig>(lhs->analogOutputStatus);
            config.time_and_interval = Conversions::ConvertConfigMap<opendnp3::TimeAndIntervalConfig>(lhs->timeAndInterval);
            config.octet_string = Conversions::ConvertConfigMap<opendnp3::OctetStringConfig>(lhs->octetString);            

            return config;
        }

        opendnp3::PointClass Conversions::Convert(PointClass clazz)
        {
            switch (clazz)
            {
            case (PointClass::Class1):
                return opendnp3::PointClass::Class1;
            case (PointClass::Class2):
                return opendnp3::PointClass::Class2;
            case (PointClass::Class3):
                return opendnp3::PointClass::Class3;
            default:
                return opendnp3::PointClass::Class0;
            }
        }

        array<System::Byte> ^ Conversions::Convert(const opendnp3::Buffer& bytes)
        {
            array<System::Byte> ^ ret = gcnew array<System::Byte>(static_cast<int>(bytes.length));

            for (int i = 0; i < ret->Length; ++i)
            {
                ret[i] = bytes.data[i];
            }

            return ret;
        }

    } // namespace Adapter
} // namespace DNP3
} // namespace Automatak
