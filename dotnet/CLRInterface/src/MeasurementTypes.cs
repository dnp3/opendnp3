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
    /// A boolean measurement type (i.e. whether a switch is open/closed)
    /// </summary>
    public class Binary : TypedMeasurementBase<bool> 
    {		
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
	    public Binary(bool value, Flags quality, DNPTime time) :
            base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Binary(bool value, Flags quality) :
            base(value, quality)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Binary(Flags quality) : base(quality.ToBinaryValue(), quality)
        { }       
    }

    /// <summary>
    /// A boolean measurement type (i.e. whether a switch is open/closed)
    /// </summary>
    public class DoubleBitBinary : TypedMeasurementBase<DoubleBit> 
    {		
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
	    public DoubleBitBinary(DoubleBit value, Flags quality, DNPTime time) : 
            base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public DoubleBitBinary(DoubleBit value, Flags quality) :
            base(value, quality)
        {}

        /// <summary>
        /// Constructor with only a quality
        /// </summary>
        /// <param name="value"></param>
        /// <param name="quality"></param>
        /// <param name="time"></param>
        public DoubleBitBinary(Flags quality)
            : base(quality.ToDoubleBitValue(), quality)
        { }
    }

    /// <summary>
    /// A discrete or continuous analog value
    /// </summary>
    public class Analog : TypedMeasurementBase<double>
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public Analog(double value, Flags quality, DNPTime time)
            : base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Analog(double value, Flags quality) : base(value, quality)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Analog(Flags quality) : base(0.0, quality)
        { }
    }

    /// <summary>
    /// An increasing count of some event
    /// </summary>
    public class Counter : TypedMeasurementBase<System.UInt32>
    {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public Counter(System.UInt32 value, Flags quality, DNPTime time): base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Counter(System.UInt32 value, Flags quality) : base(value, quality)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Counter(Flags quality) : base(0, quality)
        { }
    }

    /// <summary>
    /// An increasing count of some event frozen in time
    /// </summary>
    public class FrozenCounter : TypedMeasurementBase<System.UInt32>
    {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public FrozenCounter(System.UInt32 value, Flags quality, DNPTime time): base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public FrozenCounter(System.UInt32 value, Flags quality) : base(value, quality)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public FrozenCounter(Flags quality) : base(0, quality)
        { }
    }

    /// <summary>
    /// Represents the state of a digital output
    /// </summary>
    public class BinaryOutputStatus : TypedMeasurementBase<bool>
    {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
	    public BinaryOutputStatus(bool value, Flags quality, DNPTime time) :
            base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public BinaryOutputStatus(bool value, Flags quality) :
            base(value, quality)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public BinaryOutputStatus(Flags quality)
            : base(quality.ToBinaryValue(), quality)
        { }
    };

    /// <summary>
    /// Represents the status of analog output on an outstation.
    /// </summary>    
    public class AnalogOutputStatus : TypedMeasurementBase<double>
    {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public AnalogOutputStatus(double value, Flags quality, DNPTime time) : base(value, quality, time)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public AnalogOutputStatus(double value, Flags quality): base(value, quality)
        {}

         /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public AnalogOutputStatus(Flags quality) : base(0.0, quality)
        { }
    }

    public class OctetData
    {
        public OctetData(byte[] bytes)
        {
            if (bytes.Length < 1 || bytes.Length > 255)
            {
                throw new ArgumentException("byte array length must be in interval [1,255]", "bytes");
            }
            this.bytes = bytes;
        }

        public String AsString()
        {
            return new String(Encoding.ASCII.GetChars(bytes));
        }

        public byte[] Bytes
        {
            get { return bytes; }
        }

        private readonly byte[] bytes;    
    }

    public class OctetString : OctetData
    {
        public OctetString(byte[] bytes) : base(bytes)
        { }
    }

    public class TimeAndInterval
    {
        public TimeAndInterval(UInt64 time, UInt32 interval, Byte units)
        {
            this.time = time;
            this.interval = interval;
            this.units = units;
        }

        public TimeAndInterval(UInt64 time, UInt32 interval, IntervalUnits units)
            : this(time, interval, (Byte)units)
        {}

        IntervalUnits GetUnitsEnum()
        {
            if (Enum.IsDefined(typeof(IntervalUnits), units))
            {
                return (IntervalUnits)Enum.ToObject(typeof(IntervalUnits), units);
            }
            else
            {
                return IntervalUnits.Undefined;
            }
        }

        public override string ToString()
        {
            return String.Format("time: {0} interval: {1} units: {2}", time, interval, units);
        }

        public readonly UInt64 time;
        public readonly UInt32 interval;
        public readonly Byte units;	
    }

    /// <summary>
    /// Represents the status of analog output on an outstation.
    /// </summary>    
    public class SecurityStat : TypedMeasurementBase<System.UInt32>
    {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
        public SecurityStat(uint value, ushort assocId, Flags quality, DNPTime time)
            : base(value, quality, time)
        {
            this.assocId = assocId;
        }        

        UInt16 AssociationId
        {
            get
            {
                return assocId;
            }
        }

        private readonly UInt16 assocId;
    }

    

    public class BinaryCommandEvent
    {
        public BinaryCommandEvent(bool value, CommandStatus status, DNPTime time)
        {
            this.value = value;
            this.status = status;
            this.time = time;
        }

        public override string ToString()
        {
            return String.Format("value: {0} status: {1} time: {2}", value, status, time);
        }

        private readonly bool value;
        private readonly CommandStatus status;
        private readonly DNPTime time;

        public bool Value { get { return this.value; } }
        public CommandStatus Status { get { return this.status; } }
        public DNPTime Timestamp { get { return this.time; } }
    }

    public class AnalogCommandEvent
    {
        public AnalogCommandEvent(double value, CommandStatus status, DNPTime time)
        {
            this.value = value;
            this.status = status;
            this.time = time;
        }

        public override string ToString()
        {
            return String.Format("value: {0} status: {1} time: {2}", value, status, time);
        }

        private readonly double value;
        private readonly CommandStatus status;
        private readonly DNPTime time;

        public double Value { get { return this.value; } }
        public CommandStatus Status { get { return this.status; } }
        public DNPTime Timestamp { get { return this.time; } }
    }
}
