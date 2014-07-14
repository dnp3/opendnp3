
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Base class for most measurement types
    /// </summary>
    public abstract class MeasurementBase
    {
        /// <summary>
        /// Constructor
        /// </summary>        
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public MeasurementBase(byte quality, DateTime time, bool isTimeValid)
        {
            this.quality = quality;
            this.time = time;
            this.isTimeValid = isTimeValid;
        }

        public byte Quality
        {
            get
            {
                return quality;
            }
            set
            {
                quality = value;
            }
        }

        public DateTime Timestamp
        {
            get
            {
                return time;
            }
            set
            {
                time = value;
            }
        }

        public bool IsTimestampValid
        {
            get
            {
                return isTimeValid;
            }
        }

        public override string ToString()
        {
            string timeString = isTimeValid ? time.ToString() : "invalid";
            return "quality: " + quality.ToString("X") + " time: " + timeString;
        }

        byte quality;
        DateTime time;
        bool isTimeValid;
    }

    /// <summary>
    /// Base class for typed measurements
    /// </summary>
    public abstract class TypedMeasurementBase<T>: MeasurementBase
    {

        /// <summary>
        /// Constructor
        /// </summary>        
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public TypedMeasurementBase(T value, byte quality, DateTime time, bool isTimeValid) : 
            base(quality, time, isTimeValid)
        {
            this.value = value;         
        }

        private readonly T value;

        public T Value
        {
            get
            {
                return value;
            }
        }

        public override string ToString()
        {
            return "value: " + value + " " + base.ToString();           
        }
    }

}
