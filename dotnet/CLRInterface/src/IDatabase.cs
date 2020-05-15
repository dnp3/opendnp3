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
    /// Interface used to load measurement changes into an outstation    
    /// </summary>
    public interface IDatabase
	{

        /// <summary>
        /// Update a Binary input
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>        
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(Binary update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update a Binary input
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>        
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(DoubleBitBinary update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update an Analog input
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(Analog update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update a Counter
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(Counter update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Freeze a Counter
        /// </summary>
        /// <param name="index">index of measurement</param>
        /// <param name="clear">clear original counter</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void FreezeCounter(System.UInt16 index, bool clear, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update a BinaryOutputStatus
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(BinaryOutputStatus update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update an AnalogOutputStatus
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(AnalogOutputStatus update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update an OctetString
        /// </summary>
        /// <param name="update">measurement to update</param>
        /// <param name="index">index of measurement</param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(OctetString update, System.UInt16 index, EventMode mode = EventMode.Detect);

        /// <summary>
        /// Update a TimeAndInterval
        /// </summary>
        /// <param name="update"></param>
        /// <param name="index"></param>
        /// <param name="mode"> EventMode to use</param>
        /// <returns> true if the point exists </returns>
        void Update(TimeAndInterval update, System.UInt16 index);                
	}
}
