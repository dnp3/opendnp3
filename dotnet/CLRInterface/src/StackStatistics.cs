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
    /// Provides various counters for common transport events
    /// </summary>
    public class StackStatistics : IStackStatistics
    {
        public System.UInt64 numTransportRx = 0;
        public System.UInt64 numTransportTx = 0;
        public System.UInt64 numTransportErrorRx = 0;

        /// <summary>
        /// The number of transport frames received
        /// </summary>
        System.UInt64 IStackStatistics.NumTransportRx
        {
            get { return numTransportRx; }
        }

        /// <summary>
        /// The number of transport frames transmitted
        /// </summary>
        System.UInt64 IStackStatistics.NumTransportTx
        {
            get { return numTransportTx; }
        }

        /// <summary>
        /// The number of transport frames that caused an error
        /// </summary>
        System.UInt64 IStackStatistics.NumTransportErrorRx
        {
            get { return numTransportErrorRx; }
        }
    }
}
