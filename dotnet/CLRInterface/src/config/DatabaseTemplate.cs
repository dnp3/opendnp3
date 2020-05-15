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
    /// Defines the database layout for an outstation
    /// </summary>
    public class DatabaseTemplate
    {
        /// <summary>
        /// Constructor that sets up a number of contigous values for each type using default settings
        /// </summary>
        /// <param name="numBinary">numer of binary values starting at index 0</param>
        /// <param name="numAnalog">numer of analog values starting at index 0</param>
        /// <param name="numCounter">numer of counter values starting at index 0</param>
        /// <param name="numCounter">numer of frozen counter values starting at index 0</param>
        /// <param name="numBinaryOutputStatus">numer of control status values starting at index 0</param>
        /// <param name="numAnalogOutputStatus">numer of setpoint status values starting at index 0</param>
        /// <param name="numTimeAndInterval">numer of TimeAndInterval values starting at index 0</param>
        /// <param name="numOctetString">numer of OctetString values starting at index 0</param>
        public DatabaseTemplate(System.UInt16 numBinary,
                                System.UInt16 numDoubleBinary,
                                System.UInt16 numAnalog,
                                System.UInt16 numCounter,
                                System.UInt16 numFrozenCounter,
                                System.UInt16 numBinaryOutputStatus,
                                System.UInt16 numAnalogOutputStatus,
                                System.UInt16 numTimeAndInterval,
                                System.UInt16 numOctetString
            ) : this()
        {
            foreach(ushort index in Enumerable.Range(0, numBinary))
            {
                this.binary.Add(index, new BinaryConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numDoubleBinary))
            {
                this.doubleBinary.Add(index, new DoubleBinaryConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numAnalog))
            {
                this.analog.Add(index, new AnalogConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numCounter))
            {
                this.counter.Add(index, new CounterConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numFrozenCounter))
            {
                this.frozenCounter.Add(index, new FrozenCounterConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numBinaryOutputStatus))
            {
                this.binaryOutputStatus.Add(index, new BinaryOutputStatusConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numAnalogOutputStatus))
            {
                this.analogOutputStatus.Add(index, new AnalogOutputStatusConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numTimeAndInterval))
            {
                this.timeAndInterval.Add(index, new TimeAndIntervalConfig());
            }

            foreach (ushort index in Enumerable.Range(0, numOctetString))
            {
                this.octetString.Add(index, new OctetStringConfig());
            }
        }        

        /// <summary>
        /// Constructor that creates a contigous default count for every type of measurement
        /// </summary>
        public DatabaseTemplate(System.UInt16 count) : this(count, count, count, count, count, count, count, count, count)
        { }

        /// <summary>
        /// Default constructor that creates empty dictionaries for each type allowing full customization
        /// </summary>
        public DatabaseTemplate()
        {}

        /// <summary>
        /// Modify individual binary configuration here
        /// </summary>
        public IDictionary<ushort, BinaryConfig> binary = new Dictionary<ushort, BinaryConfig>();
        /// <summary>
        /// Modify individual double binary configuration here
        /// </summary>
        public IDictionary<ushort, DoubleBinaryConfig> doubleBinary = new Dictionary<ushort, DoubleBinaryConfig>();
        /// <summary>
        /// Modify individual analog configuration here
        /// </summary>
        public IDictionary<ushort, CounterConfig> counter = new Dictionary<ushort, CounterConfig>();
        /// <summary>
        /// Modify individual analog configuration here
        /// </summary>
        public IDictionary<ushort, FrozenCounterConfig> frozenCounter = new Dictionary<ushort, FrozenCounterConfig>();
        /// <summary>
        /// Modify individual counter configuration here
        /// </summary>
        public IDictionary<ushort, AnalogConfig> analog = new Dictionary<ushort, AnalogConfig>();
        /// <summary>
        /// Modify individual binary output status configuration here
        /// </summary>
        public IDictionary<ushort, BinaryOutputStatusConfig> binaryOutputStatus = new Dictionary<ushort, BinaryOutputStatusConfig>();
        /// <summary>
        /// Modify individual analog output status configuration here
        /// </summary>
        public IDictionary<ushort, AnalogOutputStatusConfig> analogOutputStatus = new Dictionary<ushort, AnalogOutputStatusConfig>();
        /// <summary>
        ///  Modify individual time and interval configuration here
        /// </summary>
        public IDictionary<ushort, TimeAndIntervalConfig> timeAndInterval = new Dictionary<ushort, TimeAndIntervalConfig>();
        /// <summary>
        ///  Modify individual OctetStrings configuration here
        /// </summary>
        public IDictionary<ushort, OctetStringConfig> octetString = new Dictionary<ushort, OctetStringConfig>();
    };    
}
