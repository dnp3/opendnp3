
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

namespace DNP3.Interface
{
    public static class Flags
    {
        public const byte ONLINE = 0x01;
        public const byte RESTART = 0x02;

        const byte DoubleValueMask = 0xC0;
        const byte DoubleQualityMask = 0x3F;

        public static byte FromBinaryValue(bool value, byte quality)
        {
            if (value)
            {
                return (byte) (quality | ((byte) BinaryQuality.STATE));
            }
            else
            { 
                return (byte) (quality & ~((byte) BinaryQuality.STATE));
            }
        }

        public static bool ToBinaryValue(byte quality)
        {
            return (quality & ((byte)BinaryQuality.STATE)) != 0;
        }

        public static byte FromDoubleBitValue(byte quality, DoubleBit state)
        {
	        byte value = (byte) (((byte)(state)) << 6);
            return (byte) ((DoubleQualityMask & quality) | value);
        }

        public static DoubleBit ToDoubleBitValue(byte quality)
        {
	        // the value is the top 2 bits, so downshift 6
	        byte value = (byte) ((quality >> 6) & 0xFF);
	        return (DoubleBit) value;
        }
      
    }

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
	    public Binary(bool value, byte quality, DateTime time) :
            base(value, Flags.FromBinaryValue(value, quality), time, true)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Binary(bool value, byte quality) :
            base(value, Flags.FromBinaryValue(value, quality), DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Binary(byte quality)
            : base(Flags.ToBinaryValue(quality), quality, DateTime.MinValue, false)
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
	    public DoubleBitBinary(DoubleBit value, byte quality, DateTime time) : 
            base(value, Flags.FromDoubleBitValue(quality, value), time, true)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public DoubleBitBinary(DoubleBit value, byte quality) :
            base(value, Flags.FromDoubleBitValue(quality, value), DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only a quality
        /// </summary>
        /// <param name="value"></param>
        /// <param name="quality"></param>
        /// <param name="time"></param>
        public DoubleBitBinary(byte quality)
            : base(Flags.ToDoubleBitValue(quality), quality, DateTime.MinValue, false)
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
        public Analog(double value, byte quality, DateTime time) : base(value, quality, DateTime.MinValue, false)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Analog(double value, byte quality) : base(value, quality, DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Analog(byte quality)
            : base(0.0, quality, DateTime.MinValue, false)
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
        public Counter(System.UInt32 value, byte quality, DateTime time): base(value, quality, DateTime.MinValue, false)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public Counter(System.UInt32 value, byte quality) : base(value, quality, DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public Counter(byte quality)
            : base(0, quality, DateTime.MinValue, false)
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
        public FrozenCounter(System.UInt32 value, byte quality, DateTime time): base(value, quality, DateTime.MinValue, false)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public FrozenCounter(System.UInt32 value, byte quality) : base(value, quality, DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public FrozenCounter(byte quality)
            : base(0, quality, DateTime.MinValue, false)
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
	    public BinaryOutputStatus(bool value, byte quality, DateTime time) :
            base(value, Flags.FromBinaryValue(value, quality), time, true)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public BinaryOutputStatus(bool value, byte quality) :
            base(value, Flags.FromBinaryValue(value, quality), DateTime.MinValue, false)
        {}

        /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public BinaryOutputStatus(byte quality)
            : base(Flags.ToBinaryValue(quality), quality, DateTime.MinValue, false)
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
        public AnalogOutputStatus(double value, byte quality, DateTime time) : base(value, quality, DateTime.MinValue, false)
	    {}

        /// <summary>
        /// Constructor without a timestamp
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        public AnalogOutputStatus(double value, byte quality): base(value, quality, DateTime.MinValue, false)
        {}

         /// <summary>
        /// Constructor with only quality
        /// </summary>
        /// <param name="quality"></param>
        public AnalogOutputStatus(byte quality)
            : base(0.0, quality, DateTime.MinValue, false)
        { }
    }

    public class OctetData : MeasurementBase
    {
        public OctetData(byte[] bytes) : base(0, DateTime.Now, false)
        {
            if (bytes.Length > 255) throw new ArgumentException("byte array cannot exceed length of 255", "bytes");
            this.bytes = bytes;
        }

        public String AsString()
        {
            return new String(Encoding.ASCII.GetChars(bytes));
        }

        public byte[] Bytes
        {
            get { return Bytes; }
        }

        private readonly byte[] bytes;    
    }

    public class OctetString : OctetData
    {
        public OctetString(byte[] bytes) : base(bytes)
        { }
    }

}
