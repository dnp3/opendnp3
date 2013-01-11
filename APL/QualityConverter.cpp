//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "QualityConverter.h"

namespace apl
{


template<> const uint8_t BinaryQualInfo::masks[] = {BQ_ONLINE, BQ_RESTART, BQ_COMM_LOST, BQ_REMOTE_FORCED_DATA, BQ_LOCAL_FORCED_DATA, BQ_CHATTER_FILTER};
template<> const std::string BinaryQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced", "LocalForced", "ChatterFilter"};
template<> const char BinaryQualInfo::symbols[] = {'O', 'R', 'C', 'F', 'L', 'T'};

template<> const uint8_t AnalogQualInfo::masks[] = {AQ_ONLINE, AQ_RESTART, AQ_COMM_LOST, AQ_REMOTE_FORCED_DATA, AQ_LOCAL_FORCED_DATA, AQ_OVERRANGE, AQ_REFERENCE_CHECK};
template<> const std::string AnalogQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced", "LocalForced", "Overrange", "ReferenceCheck"};
template<> const char AnalogQualInfo::symbols[] = {'O', 'R', 'C', 'F', 'L', 'V', 'K'};

template<> const uint8_t CounterQualInfo::masks[] = {CQ_ONLINE, CQ_RESTART, CQ_COMM_LOST, CQ_REMOTE_FORCED_DATA, CQ_LOCAL_FORCED_DATA, CQ_ROLLOVER, CQ_DISCONTINUITY};
template<> const std::string CounterQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced", "LocalForced", "Rollover", "Discontinuity"};
template<> const char CounterQualInfo::symbols[] = {'O', 'R', 'C', 'F', 'L', 'V', 'D'};

template<> const uint8_t ControlQualInfo::masks[] = {TQ_ONLINE, TQ_RESTART, TQ_COMM_LOST, TQ_REMOTE_FORCED_DATA, TQ_LOCAL_FORCED_DATA};
template<> const std::string ControlQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced", "LocalForced"};
template<> const char ControlQualInfo::symbols[] = {'O', 'R', 'C', 'F', 'L'};

template<> const uint8_t SetpointQualInfo::masks[] = {PQ_ONLINE, PQ_RESTART, PQ_COMM_LOST, PQ_REMOTE_FORCED_DATA};
template<> const std::string SetpointQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced"};
template<> const char SetpointQualInfo::symbols[] = {'O', 'R', 'C', 'F'};

template<> const uint8_t VtoQualInfo::masks[] = {VQ_ONLINE, VQ_RESTART, VQ_COMM_LOST, VQ_REMOTE_FORCED_DATA};
template<> const std::string VtoQualInfo::names[] = {"Online", "Restart", "CommsLost", "RemoteForced"};
template<> const char VtoQualInfo::symbols[] = {'O', 'R', 'C', 'F'};

}
