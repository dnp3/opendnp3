// Copyright 2013-2019 Automatak, LLC
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
    /// Base class for most measurement types
    /// </summary>
    public abstract class MeasurementBase
    {
        /// <summary>
        /// Constructor
        /// </summary>        
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public MeasurementBase(byte quality, DateTime time)
        {
            this.quality = quality;
            this.time = time;            
        }

        public MeasurementBase(byte quality)
        {
            this.quality = quality;
            this.time = DateTime.MinValue;
        }

        public MeasurementBase()
        {
            this.quality = 0;
            this.time = DateTime.MinValue;
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

        public override string ToString()
        {            
            return "quality: " + quality.ToString("X") + " time: " + time.ToString();
        }

        byte quality;
        DateTime time;        
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
        public TypedMeasurementBase(T value, byte quality, DateTime time) :  base(quality, time)
        {
            this.value = value;         
        }

        public TypedMeasurementBase(T value, byte quality) :  base(quality)            
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
