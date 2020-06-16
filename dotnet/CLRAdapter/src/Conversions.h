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
#ifndef OPENDNP3CLR_CONVERSIONS_H
#define OPENDNP3CLR_CONVERSIONS_H

#include <opendnp3/StackStatistics.h>

#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/gen/LinkStatus.h>

#include <opendnp3/link/LinkStatistics.h>
#include <opendnp3/link/LinkHeaderFields.h>

#include <opendnp3/logging/LogLevels.h>

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>
#include <opendnp3/app/AnalogOutput.h>
#include <opendnp3/app/OctetString.h>
#include <opendnp3/app/Indexed.h>
#include <opendnp3/app/BinaryCommandEvent.h>
#include <opendnp3/app/AnalogCommandEvent.h>
#include <opendnp3/app/parsing/ICollection.h>
#include <opendnp3/app/GroupVariationID.h>

#include <opendnp3/master/ICommandTaskResult.h>
#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/X509Info.h>
#include <opendnp3/outstation/OutstationStackConfig.h>

#include <opendnp3/channel/ChannelRetry.h>
#include <opendnp3/channel/IPEndpoint.h>
#include <opendnp3/channel/TLSConfig.h>
#include <opendnp3/channel/SerialSettings.h>

#include "CollectionAdapter.h"

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::Generic;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class Conversions
            {
            public:

                static opendnp3::TimeDuration ConvertMilliseconds(System::Int64 ms);

                static opendnp3::TimeDuration ConvertTimespan(System::TimeSpan ts);

                static System::TimeSpan ConvertTimeDuration(const opendnp3::TimeDuration& duration);

                static opendnp3::ClassField ConvertClassField(ClassField classField);

                static opendnp3::TLSConfig Convert(Automatak::DNP3::Interface::TLSConfig^ config);

                // Convert a .NET string to a C++ string
                static std::string ConvertString(System::String^ s);
                static System::String^ ConvertString(const std::string& s);

                // Converting channel state enumeration
                static ChannelState ConvertChannelState(opendnp3::ChannelState aState);

                static LinkStatus ConvertLinkStatus(opendnp3::LinkStatus aState);

                static IChannelStatistics^ ConvertChannelStats(const opendnp3::LinkStatistics& statistics);

                static IStackStatistics^ ConvertStackStats(const opendnp3::StackStatistics& statistics);

                // Convert the command status enumeration
                static CommandStatus ConvertCommandStatus(opendnp3::CommandStatus status);
                static opendnp3::CommandStatus ConvertCommandStatus(CommandStatus status);

                static CommandTaskResult^ ConvertCommandTaskResult(const opendnp3::ICommandTaskResult& result);

                //functions for Converting binary outputs

                static ControlRelayOutputBlock^ ConvertCommand(const opendnp3::ControlRelayOutputBlock& bo);
                static opendnp3::ControlRelayOutputBlock ConvertCommand(ControlRelayOutputBlock^ bo);

                //functions for Converting setpoints

                static opendnp3::AnalogOutputInt32 ConvertCommand(AnalogOutputInt32^ sp);
                static AnalogOutputInt32^ ConvertCommand(const opendnp3::AnalogOutputInt32& sp);

                static opendnp3::AnalogOutputInt16 ConvertCommand(AnalogOutputInt16^ sp);
                static AnalogOutputInt16^ ConvertCommand(const opendnp3::AnalogOutputInt16& sp);

                static opendnp3::AnalogOutputFloat32 ConvertCommand(AnalogOutputFloat32^ sp);
                static AnalogOutputFloat32^ ConvertCommand(const opendnp3::AnalogOutputFloat32& sp);

                static opendnp3::AnalogOutputDouble64 ConvertCommand(AnalogOutputDouble64^ sp);
                static AnalogOutputDouble64^ ConvertCommand(const opendnp3::AnalogOutputDouble64& sp);

                //functions for Converting Measurement types
                static Binary^ ConvertMeas(opendnp3::Binary meas);
                static DoubleBitBinary^ ConvertMeas(opendnp3::DoubleBitBinary meas);
                static Analog^ ConvertMeas(opendnp3::Analog meas);
                static Counter^ ConvertMeas(opendnp3::Counter meas);
                static FrozenCounter^ ConvertMeas(opendnp3::FrozenCounter meas);
                static AnalogOutputStatus^ ConvertMeas(opendnp3::AnalogOutputStatus meas);
                static BinaryOutputStatus^ ConvertMeas(opendnp3::BinaryOutputStatus meas);
                static OctetString^ Conversions::ConvertMeas(const opendnp3::OctetString& meas);
                static TimeAndInterval^ Conversions::ConvertMeas(const opendnp3::TimeAndInterval& meas);
                static BinaryCommandEvent^ Conversions::ConvertMeas(const opendnp3::BinaryCommandEvent& meas);
                static AnalogCommandEvent^ Conversions::ConvertMeas(const opendnp3::AnalogCommandEvent& meas);

                static opendnp3::DNPTime ConvertTime(DNPTime^ time);
                static DNPTime^ ConvertTime(opendnp3::DNPTime time);

                static opendnp3::Binary ConvertMeas(Binary^ meas);
                static opendnp3::DoubleBitBinary ConvertMeas(DoubleBitBinary^ meas);
                static opendnp3::Analog ConvertMeas(Analog^ meas);
                static opendnp3::Counter ConvertMeas(Counter^ meas);
                static opendnp3::FrozenCounter ConvertMeas(FrozenCounter^ meas);
                static opendnp3::AnalogOutputStatus ConvertMeas(AnalogOutputStatus^ meas);
                static opendnp3::BinaryOutputStatus ConvertMeas(BinaryOutputStatus^ meas);
                static opendnp3::TimeAndInterval ConvertMeas(TimeAndInterval^ meas);
                static opendnp3::OctetString ConvertMeas(OctetString^ meas);
                static opendnp3::BinaryCommandEvent ConvertMeas(BinaryCommandEvent^ meas);
                static opendnp3::AnalogCommandEvent ConvertMeas(AnalogCommandEvent^ meas);

                static LinkHeader^ Conversions::Convert(const opendnp3::LinkHeaderFields& fields);
                static opendnp3::IPEndpoint Convert(IPEndpoint^ endpoint);

                static X509Info^ Convert(const opendnp3::X509Info& info);

                //Convert the configuration types
                static opendnp3::SerialSettings ConvertSerialSettings(SerialSettings^ settings);
                static opendnp3::EventBufferConfig ConvertConfig(EventBufferConfig^ cm);

                static opendnp3::LinkConfig ConvertConfig(LinkConfig^ config);
                static opendnp3::MasterParams ConvertConfig(MasterConfig^ config);
                static opendnp3::OutstationConfig ConvertConfig(OutstationConfig^ config);
                static opendnp3::OutstationParams ConvertConfig(OutstationParams^ config);
                static opendnp3::MasterStackConfig ConvertConfig(MasterStackConfig^ config);
                static opendnp3::OutstationStackConfig ConvertConfig(OutstationStackConfig^ config);
                static opendnp3::NumRetries ConvertNumRetries(NumRetries^ numRetries);

                static opendnp3::PointClass Convert(PointClass clazz);
                static array<System::Byte>^ Convert(const opendnp3::Buffer& bytes);

                template <class Target, class Source>
                static IndexedValue<Target>^ ConvertIndexValue(const opendnp3::Indexed<Source>& pair)
                {
                    return gcnew IndexedValue<Target>(ConvertMeas(pair.value), pair.index);
                }

                template <class Target, class Source>
                static IEnumerable<IndexedValue<Target>^>^ ToIndexedEnumerable(const opendnp3::ICollection<opendnp3::Indexed<Source>>& values)
                {
                    auto convert = [](const opendnp3::Indexed <Source> &value) -> IndexedValue<Target>^ {
                        return ConvertIndexValue<Target, Source>(value);
                    };

                    auto adapter = CreateAdapter<opendnp3::Indexed<Source>, IndexedValue<Target>^>(convert);

                    values.Foreach(adapter);
                    return adapter.GetValues();
                }

                private:

                    static opendnp3::DatabaseConfig Convert(DatabaseTemplate^ lhs);

                    static opendnp3::BinaryConfig ConvertPointConfig(BinaryConfig^ config) {
                        return ConvertEventConfig<opendnp3::BinaryInfo, opendnp3::BinaryConfig>(config);
                    }

                    static opendnp3::DoubleBitBinaryConfig ConvertPointConfig(DoubleBinaryConfig^ config) {
                        return ConvertEventConfig<opendnp3::DoubleBitBinaryInfo, opendnp3::DoubleBitBinaryConfig>(config);
                    }

                    static opendnp3::BOStatusConfig ConvertPointConfig(BinaryOutputStatusConfig^ config) {
                        return ConvertEventConfig<opendnp3::BinaryOutputStatusInfo, opendnp3::BOStatusConfig>(config);
                    }

                    static opendnp3::AnalogConfig ConvertPointConfig(AnalogConfig^ config) {
                        return ConvertDeadbandConfig<opendnp3::AnalogInfo, opendnp3::AnalogConfig>(config);
                    }

                    static opendnp3::CounterConfig ConvertPointConfig(CounterConfig^ config) {
                        return ConvertDeadbandConfig<opendnp3::CounterInfo, opendnp3::CounterConfig>(config);
                    }

                    static opendnp3::FrozenCounterConfig ConvertPointConfig(FrozenCounterConfig^ config) {
                        return ConvertDeadbandConfig<opendnp3::FrozenCounterInfo, opendnp3::FrozenCounterConfig>(config);
                    }

                    static opendnp3::AOStatusConfig ConvertPointConfig(AnalogOutputStatusConfig^ config) {
                        return ConvertDeadbandConfig<opendnp3::AnalogOutputStatusInfo, opendnp3::AOStatusConfig>(config);
                    }

                    static opendnp3::TimeAndIntervalConfig ConvertPointConfig(TimeAndIntervalConfig^ config) {
                        opendnp3::TimeAndIntervalConfig ret;
                        ret.svariation = static_cast<opendnp3::StaticTimeAndIntervalVariation>(config->staticVariation);
                        return ret;
                    }

                    static opendnp3::OctetStringConfig ConvertPointConfig(OctetStringConfig^ config) {
                        return ConvertEventConfig<opendnp3::OctetStringInfo, opendnp3::OctetStringConfig>(config);
                    }

                    template <class Info, class Target, class Source>
                    static typename Target ConvertEventConfig(Source^ config) {
                        Target ret;
                        ret.clazz = static_cast<opendnp3::PointClass>(config->clazz);
                        ret.evariation = static_cast<Info::event_variation_t>(config->eventVariation);
                        ret.svariation = static_cast<Info::static_variation_t>(config->staticVariation);
                        return ret;

                    }

                    template <class Info, class Target, class Source>
                    static typename Target ConvertDeadbandConfig(Source^ config) {
                        Target ret;
                        ret.clazz = static_cast<opendnp3::PointClass>(config->clazz);
                        ret.evariation = static_cast<Info::event_variation_t>(config->eventVariation);
                        ret.svariation = static_cast<Info::static_variation_t>(config->staticVariation);
                        ret.deadband = static_cast<Info::value_t>(config->deadband);
                        return ret;
                    }

                    template <class Target, class Source>
                    static std::map<uint16_t, Target> ConvertConfigMap(System::Collections::Generic::IDictionary<System::UInt16, Source^>^ source)
                    {
                        std::map<uint16_t, Target> ret;
                        for each(KeyValuePair<System::UInt16, Source^>^ kvp in source) {
                            ret[kvp->Key] = ConvertPointConfig(kvp->Value);
                        }
                        return ret;
                    }
            };

        }
    }
}

#endif
