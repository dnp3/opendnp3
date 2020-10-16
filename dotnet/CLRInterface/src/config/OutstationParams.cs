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
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Configuration information for a dnp3 outstation (outstation)
    /// Used as both input describing the startup configuration of the outstation, and as configuration state of mutable properties (i.e. unsolicited responses).
    /// Major feature areas are unsolicited responses, time synchronization requests, event buffer limits, and the DNP3 object/variations to use by default
    /// when the master requests class data or variation 0.
    /// </summary>
    public class OutstationParams
    {        
        /// <summary>
        /// The maximum number of controls the outstation will attempt to process from a single APDU
        /// </summary>
        public System.UInt32 maxControlsPerRequest = System.UInt32.MaxValue;

        /// <summary>
        ///  How long the outstation will allow an operate to proceed after a prior select
        /// </summary>
        public TimeSpan selectTimeout = TimeSpan.FromSeconds(10);

        /// <summary>
        /// Timeout for solicited confirms
        /// </summary>
        public TimeSpan solicitedConfirmTimeout = TimeSpan.FromSeconds(5);

        /// <summary>
        /// Timeout for unsolicited confirms
        /// </summary>
        public TimeSpan unsolConfirmTimeout = TimeSpan.FromSeconds(5);

        /// <summary>
        /// Number of unsolicited response retries
        /// </summary>
        public NumRetries numUnsolRetries = NumRetries.Infinite();

        /// <summary>
        /// The maximum fragment size the outstation will use for fragments it sends
        /// </summary>
        public System.UInt32 maxTxFragSize = 2048;

        /// <summary>
        /// The maximum fragment size the outstation will be able to receive
        /// </summary>
        public System.UInt32 maxRxFragSize = 2048;

        /// <summary>
        /// Global enabled / disable for unsolicted messages. If false, the NULL unsolicited message is not even sent
        /// </summary>
        public bool allowUnsolicited = true;

        /// <summary>
        /// Specifies which types are allowed in Class0 repsones. Defaults to all types.
        /// Use this field to disable some types for compatibility reasons.
        /// </summary>
        public StaticTypeBitField typesAllowedInClass0 = StaticTypeBitField.AllTypes();

        /// <summary>
        /// Workaround for test procedure 8.11.2.1. Will respond immediatly to READ requests
        /// when waiting for unsolicited NULL responses.
        /// This is NOT compliant to IEEE 1815-2012.
        /// </summary>
        public bool noDefferedReadDuringUnsolicitedNullResponse = false;

        /// <summary>
        /// Class mask for unsolicted, default to 0 as unsolicited has to be enabled by master
        /// </summary>
        public ClassField unsolClassMask = ClassField.None;

        /// <summary>
        /// If true, the outstation and link-layer will respond to any source address
        /// </summary>
        public bool respondToAnyMaster = false;
    }  
}
