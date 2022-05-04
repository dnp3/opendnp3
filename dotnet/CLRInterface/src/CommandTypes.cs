// Copyright 2013-2022 Step Function I/O, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

namespace Step Function I/O.DNP3.Interface
{      
    /// <summary>
    /// Command request to control a relay output.
    /// 
    /// Has lots of possible modes. Consult the dnp3 standard for all valid combinations.
    /// </summary>
    public class ControlRelayOutputBlock
    {
        /// <summary>
        /// Primary constructor
        /// </summary>
        /// <param name="opType">Enumeration that controls the behavior of the command</param>
        /// <param name="tcc">Enumeration that controls the behavior of the command</param>
        /// <param name="clear">Boolean that controls the behavior of the command</param>
        /// <param name="count">How many times to repeat the operation</param>
        /// <param name="onTime">How long to set the output active></param>
        /// <param name="offTime">How long to set the output inactive></param>
        public ControlRelayOutputBlock(OperationType opType, TripCloseCode tcc, bool clear, byte count, System.UInt32 onTime, System.UInt32 offTime)
        {
            this.opType = opType;
            this.tcc = tcc;
            this.clear = clear;
            this.count = count;
            this.onTime = onTime;
            this.offTime = offTime;
        }
    
        public readonly OperationType opType;
        public readonly TripCloseCode tcc;
        public readonly bool clear;
        public readonly byte count;
        public readonly System.UInt32 onTime;
        public readonly System.UInt32 offTime;
    }

    /// <summary>
    /// Base class for all AnalogOutputs
    /// </summary>
    /// <typeparam name="T">Type of value sent to the outstation</typeparam>
    public class AnalogOutput<T>
    {	
    	
	    public AnalogOutput(T value)
        {
            this.value = value;            
        }	   	    

	    public readonly T value;        
    }
   
    /// <summary>
    /// A set-point command request with short integer representation.
    /// The underlying DNP3 serialization is Group41, Variation 1
    /// </summary>
    public class AnalogOutputInt32 : AnalogOutput<System.Int32>
    {
        public AnalogOutputInt32(System.Int32 value) : base(value)
        {}
    }

    /// <summary>
    /// A set-point command request with short integer representation.
    /// The underlying DNP3 serialization is Group41, Variation 2
    /// </summary>
    public class AnalogOutputInt16 : AnalogOutput<System.Int16>
    {
        public AnalogOutputInt16(System.Int16 value)
            : base(value)
        { }
    }

    /// <summary>
    /// A set-point command request with single precision floating point representation.
    /// The underlying DNP3 serialization is Group41, Variation 3
    /// </summary>
    public class AnalogOutputFloat32 : AnalogOutput<float>
    {
        public AnalogOutputFloat32(float value)
            : base(value)
        { }
    }

    /// <summary>
    /// A set-point command request with double precision floating point representation.
    /// The underlying DNP3 serialization is Group41, Variation 4
    /// </summary>
    public class AnalogOutputDouble64 : AnalogOutput<double>
    {
        public AnalogOutputDouble64(System.Double value)
            : base(value)
        { }
    }

}
