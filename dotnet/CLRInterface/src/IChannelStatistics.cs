// Copyright 2013-2020 Automatak, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Statistics for a channel
    /// </summary>
    public interface IChannelStatistics
    {
        /// <summary>
        /// The number of times the channel has sucessfully opened
        /// </summary>
        System.UInt64 NumOpen { get; }

        /// <summary>
        /// The number of times the channel has failed to open
        /// </summary>
        System.UInt64 NumOpenFail { get; }

        /// <summary>
        /// The number of times the channel has closed either due to user intervention or an error
        /// </summary>
        System.UInt64 NumClose { get; }

        /// <summary>
        /// The number of bytes received
        /// </summary>
        System.UInt64 NumBytesRx { get; }

        /// <summary>
        /// The number of bytes transmitted
        /// </summary>
        System.UInt64 NumBytesTx { get; }

        /// <summary>
        /// Number of frames discared due to CRC errors
        /// </summary>
        System.UInt64 NumCrcError { get; }

        /// <summary>
        /// Number of valid LPDUs received
        /// </summary>
        System.UInt64 NumLinkFrameRx { get; }

        /// <summary>
        /// Number of valid LPDUs transmitted
        /// </summary>
        System.UInt64 NumLinkFrameTx { get; }

        /// <summary>
        /// Number of LPDUs detected with bad / malformed contents
        /// </summary>
        System.UInt64 NumBadLinkFrameRx { get; }
    }
}
