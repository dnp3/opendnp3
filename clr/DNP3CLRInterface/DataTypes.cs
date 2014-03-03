
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
    /// <summary>
    /// Quality enumeration for Binary measurements
    /// </summary>
    public enum BinaryQuality {
        /// <summary>
        /// set when the data is "good", meaning that rest of the system can trust the value
        /// </summary>
	    BQ_ONLINE  = 0x01,
        /// <summary>
        /// the quality all points get before we have established communication (or populated) the point
        /// </summary>
	    BQ_RESTART = 0x02,
        /// <summary>
        /// this means the communication has been lost with the source of the data (after establishing contact)
        /// </summary>
	    BQ_COMM_LOST = 0x04,
        /// <summary>
        /// the value is being forced to a "fake" value somewhere in the system
        /// </summary>
	    BQ_REMOTE_FORCED_DATA = 0x08,
        /// <summary>
        /// the value is being forced to a "fake" value on the original device
        /// </summary>
	    BQ_LOCAL_FORCED_DATA = 0x10,
        /// <summary>
        /// set when the value is osciallating very quickly and some events are being suppressed
        /// </summary>
	    BQ_CHATTER_FILTER = 0x20,
        /// <summary>
        /// reserved
        /// </summary>
	    BQ_RESERVED = 0x40,
        /// <summary>
        /// the actual value of the binary
        /// </summary>
	    BQ_STATE = 0x80
    }

    /// <summary>
    /// A boolean measurement type (i.e. whether a switch is open/closed)
    /// </summary>
    public class Binary {
		
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="value">value of the measurement</param>
        /// <param name="quality">quality enumeration as a bitfield</param>
        /// <param name="time">timestamp</param>
	    public Binary(bool value, byte quality, DateTime time)
	    {
            this.value = value;
            this.quality = quality;
            this.time = time;
        }

        public readonly bool value;
        public readonly byte quality;
        public readonly DateTime time;
    }

/// <summary>
/// Bitmasks that make make up the quality field for analogs. See BinaryQuality for common (unlabeled) bitmasks.
/// </summary>
public enum AnalogQuality {
	AQ_ONLINE = 0x01,
	AQ_RESTART = 0x02,
	AQ_COMM_LOST = 0x04,
	AQ_REMOTE_FORCED_DATA = 0x08,
	AQ_LOCAL_FORCED_DATA = 0x10,
    /// <summary>
    /// if a hardware input etc. is out of range and we are using a place holder value
    /// </summary>
	AQ_OVERRANGE = 0x20,
    /// <summary>
    /// meaning we may have lost calibration or refrence voltage so readings are questionable
    /// </summary>
	AQ_REFERENCE_CHECK = 0x40,
	AQ_RESERVED = 0x80
};

/// <summary>
/// A discrete or continuous analog value
/// </summary>
public class Analog {

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="value">value of the measurement</param>
    /// <param name="quality">quality enumeration as a bitfield</param>
    /// <param name="time">timestamp</param>
    public Analog(double value, byte quality, DateTime time)
	{
        this.value = value;
        this.quality = quality;
        this.time = time;
    }

    public readonly double value;
    public readonly byte quality;
    public readonly DateTime time;
}

/// <summary>
/// Quality enumeration for Counters
/// </summary>
public enum CounterQuality {
	CQ_ONLINE = 0x01,
	CQ_RESTART = 0x02,
	CQ_COMM_LOST = 0x04,
	CQ_REMOTE_FORCED_DATA = 0x08,
	CQ_LOCAL_FORCED_DATA = 0x10,
    /// <summary>
    /// used to indicate that the counter filled up and rolledover, cleared automatically after reading
    /// </summary>
	CQ_ROLLOVER = 0x20,
    /// <summary>
    /// indicates an unusual change in value
    /// </summary>
	CQ_DISCONTINUITY = 0x40,
	CQ_RESERVED = 0x80
}

/// <summary>
/// An increasing count of some event
/// </summary>
public class Counter {

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="value">value of the measurement</param>
    /// <param name="quality">quality enumeration as a bitfield</param>
    /// <param name="time">timestamp</param>
    public Counter(System.UInt32 value, byte quality, DateTime time)
	{
        this.value = value;
        this.quality = quality;
        this.time = time;
    }

    public readonly System.UInt32 value;
    public readonly byte quality;
    public readonly DateTime time;
}

/// <summary>
/// Quality enumeration for FrozenCounters
/// </summary>
public enum FrozenCounterQuality
{
    CQ_ONLINE = 0x01,
    CQ_RESTART = 0x02,
    CQ_COMM_LOST = 0x04,
    CQ_REMOTE_FORCED_DATA = 0x08,
    CQ_LOCAL_FORCED_DATA = 0x10,
    /// <summary>
    /// used to indicate that the counter filled up and rolledover, cleared automatically after reading
    /// </summary>
    CQ_ROLLOVER = 0x20,
    /// <summary>
    /// indicates an unusual change in value
    /// </summary>
    CQ_DISCONTINUITY = 0x40,
    CQ_RESERVED = 0x80
}

/// <summary>
/// An increasing count of some event frozen in time
/// </summary>
public class FrozenCounter
{

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="value">value of the measurement</param>
    /// <param name="quality">quality enumeration as a bitfield</param>
    /// <param name="time">timestamp</param>
    public FrozenCounter(System.UInt32 value, byte quality, DateTime time)
    {
        this.value = value;
        this.quality = quality;
        this.time = time;
    }

    public readonly System.UInt32 value;
    public readonly byte quality;
    public readonly DateTime time;
}

/// <summary>
/// Quality enumeration for Control Status measurements
/// </summary>
public enum ControlQuality {
	TQ_ONLINE = 0x01,
	TQ_RESTART = 0x02,
	TQ_COMM_LOST = 0x04,
	TQ_REMOTE_FORCED_DATA = 0x08,
	TQ_LOCAL_FORCED_DATA = 0x10,
	TQ_RESERVED_1 = 0x20,
	TQ_RESERVED_2 = 0x40,
	TQ_STATE = 0x80
}

/// <summary>
/// Represents the state of a digital output
/// </summary>
public class BinaryOutputStatus {

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="value">value of the measurement</param>
    /// <param name="quality">quality enumeration as a bitfield</param>
    /// <param name="time">timestamp</param>
    public BinaryOutputStatus(bool value, byte quality, DateTime time)
	{
        this.value = value;
        this.quality = quality;
        this.time = time;
    }

    public readonly bool value;
    public readonly byte quality;
    public readonly DateTime time;
};

/// <summary>
/// Quality enumeration for AnalogOutputStatus measurements
/// </summary>
public enum SetpointQuality {
	PQ_ONLINE = 0x01,
	PQ_RESTART = 0x02,
	PQ_COMM_LOST = 0x04,
	PQ_REMOTE_FORCED_DATA = 0x08,
	PQ_RESERVED_1 = 0x10,
	PQ_RESERVED_2 = 0x20,
	PQ_RESERVED_3 = 0x40,
	PQ_RESERVED_4 = 0x80,
}

/// <summary>
/// Represents the status of analog output on an outstation.
/// </summary>    
public class AnalogOutputStatus {

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="value">value of the measurement</param>
    /// <param name="quality">quality enumeration as a bitfield</param>
    /// <param name="time">timestamp</param>
    public AnalogOutputStatus(double value, byte quality, DateTime time)
	{
        this.value = value;
        this.quality = quality;
        this.time = time;        
    }
	
	public readonly double value;
    public readonly byte quality;
    public readonly DateTime time;
}

public class OctetData
{
    public OctetData(byte[] bytes)
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
