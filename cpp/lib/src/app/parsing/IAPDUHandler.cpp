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
#include "IAPDUHandler.h"

namespace opendnp3
{

IAPDUHandler::IAPDUHandler() : numTotalHeaders(0), numIgnoredHeaders(0) {}

void IAPDUHandler::Reset()
{
    numTotalHeaders = 0;
    numIgnoredHeaders = 0;
    errors.Clear();
}

IINField IAPDUHandler::Errors() const
{
    return errors;
}

void IAPDUHandler::OnHeader(const AllObjectsHeader& header)
{
    Record(header, this->ProcessHeader(header));
}

void IAPDUHandler::OnHeader(const RangeHeader& header)
{
    Record(header, this->ProcessHeader(header));
}

void IAPDUHandler::OnHeader(const CountHeader& header)
{
    Record(header, this->ProcessHeader(header));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group50Var1>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group50Var3>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group51Var1>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group51Var2>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group52Var1>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const CountHeader& header, const ICollection<Group52Var2>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

// --- index prefixes ----

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<uint16_t>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

// --- controls ----

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

void IAPDUHandler::OnHeader(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
    Record(header, this->ProcessHeader(header, values));
}

IINField IAPDUHandler::ProcessHeader(const AllObjectsHeader& /*record*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/)
{
    return ProcessUnsupportedHeader();
}

/// ---- counts -----

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group50Var1>& /*unused*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group50Var3>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group51Var1>& /*unused*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group51Var2>& /*unused*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var1>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group52Var2>& /*unused*/)
{
    return ProcessUnsupportedHeader();
}

/// ---- ranges -----

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<IINValue>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<Binary>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/,
                                     const ICollection<Indexed<DoubleBitBinary>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/,
                                     const ICollection<Indexed<BinaryOutputStatus>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<Counter>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/,
                                     const ICollection<Indexed<FrozenCounter>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<Analog>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputStatus>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<OctetString>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const RangeHeader& /*header*/,
                                     const ICollection<Indexed<TimeAndInterval>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

/// ---- index prefixes -----

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/, const ICollection<Indexed<Counter>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<FrozenCounter>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/, const ICollection<Indexed<Binary>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<BinaryOutputStatus>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<DoubleBitBinary>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/, const ICollection<Indexed<Analog>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputStatus>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<OctetString>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<TimeAndInterval>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<BinaryCommandEvent>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogCommandEvent>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/, const ICollection<uint16_t>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

//// --- controls ----

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<ControlRelayOutputBlock>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputInt16>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputInt32>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputFloat32>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

IINField IAPDUHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<AnalogOutputDouble64>>& /*values*/)
{
    return ProcessUnsupportedHeader();
}

} // namespace opendnp3
