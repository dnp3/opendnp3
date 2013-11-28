
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
    /// Configuration class for the link layer
    /// </summary>
    public class LinkConfig
	{
		/// <summary>
		/// Full constructor
		/// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
        /// <param name="numRetry">The number of retry attempts the link will attempt after the initial try</param>
        /// <param name="localAddr">dnp3 address of the local device</param>
        /// <param name="remoteAddr">dnp3 address of the remote device</param>
        /// <param name="timeout">the response timeout in milliseconds for confirmed requests</param>
		public LinkConfig(	bool isMaster, 
						    bool useConfirms, 
						    System.UInt32 numRetry, 
						    System.UInt16 localAddr, 
						    System.UInt16 remoteAddr, 
						    TimeSpan timeout)							
		{
            this.isMaster = isMaster;
            this.useConfirms = useConfirms;
            this.numRetry = numRetry;
            this.localAddr = localAddr;
            this.remoteAddr = remoteAddr;
            this.timeout = timeout;
        }

        /// <summary>
        /// Defaults constructor
        /// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
		public LinkConfig(bool isMaster, bool useConfirms)
        {
			this.isMaster = isMaster;
			this.useConfirms = useConfirms;
			this.numRetry = 0;
			this.localAddr = (ushort) (isMaster ? 1 : 1024);
			this.remoteAddr = (ushort) (isMaster ? 1024 : 1);
            this.timeout = TimeSpan.FromSeconds(1);
		}        

		/// <summary>
		/// The master/slave bit set on all messages
		/// </summary>
		public bool isMaster;

		/// <summary>
		/// If true, the link layer will send data requesting confirmation
		/// </summary> 
		public bool useConfirms;

		/// <summary>
		/// The number of retry attempts the link will attempt after the initial try
		/// </summary>
		public System.UInt32 numRetry;

		/// <summary>
		/// dnp3 address of the local device
		/// </summary>
		public System.UInt16 localAddr;

		/// <summary>
		/// dnp3 address of the remote device
		/// </summary>
		public System.UInt16 remoteAddr;

		/// <summary>
		/// the response timeout for confirmed requests
		/// </summary>
		public TimeSpan timeout;
	}

    /// <summary>
    /// Configuration object for the application layer
    /// </summary>
	public class AppConfig
	{	
	    /// <summary>
	    /// Constructor with reasonable defaults
	    /// </summary>
		public AppConfig()
        {
            this.rspTimeout = TimeSpan.FromSeconds(5);
            this.numRetry = 0;
            this.fragSize = 2048;
        }

        /// <summary>
        /// Constructor with all values parameterized
        /// </summary>
        /// <param name="rspTimeout"> The response/confirm timeout in millisec</param>
        /// <param name="numRetry">Number of retries performed for applicable frames</param>
        /// <param name="fragSize">The maximum size of received application layer fragments</param>
		public AppConfig(TimeSpan rspTimeout, System.Int32 numRetry, System.Int32 fragSize)
        {
			this.rspTimeout = rspTimeout;
			this.numRetry = numRetry;
			this.fragSize = fragSize;
        }
		

		/// <summary>
		/// The response/confirm timeout in millisec
		/// </summary>
		public TimeSpan rspTimeout;

		/// <summary>
		/// Number of retries performed for applicable frames
		/// </summary>
        public System.Int32 numRetry;

		/// <summary>
		/// The maximum size of received application layer fragments
		/// </summary>
        public System.Int32 fragSize;
	}
	
    /// <summary>
    /// Enumeration for controlling class based scanning / eventing
    /// CLASS 0 corresponds to Group 60 Variation 1
    /// CLASS 1 corresponds to Group 60 Variation 2
    /// CLASS 2 corresponds to Group 60 Variation 3
    /// CLASS 3 corresponds to Group 60 Variation 4
    /// </summary>
	public enum PointClass {
		PC_CLASS_0 = 0x01,
		PC_CLASS_1 = 0x02,
		PC_CLASS_2 = 0x04,
		PC_CLASS_3 = 0x08,
		PC_ALL_EVENTS = PC_CLASS_1 | PC_CLASS_2 | PC_CLASS_3,
        PC_INVALID = 0x10       
	}

    public static class PointClassHelpers
    {
        public static int GetMask(params PointClass[] values)
        {
            var i = 0;
            foreach(var e in values) i |= (int) e;
            return i;
        }        
    }

	/// <summary>
    /// Configuration information for the dnp3 master
	/// </summary>
	public class MasterConfig {
		
		/// <summary>
		/// Constructor with reasonable defaults
		/// </summary>
		public MasterConfig()
        {
			fragSize = 2048;
			allowTimeSync = true;
			doUnsolOnStartup = false;
			enableUnsol = true;
			unsolClassMask = (System.Int32) (PointClass.PC_ALL_EVENTS);
            integrityPeriod = TimeSpan.FromSeconds(5);
            taskRetryPeriod = TimeSpan.FromSeconds(5);
		}		

		/// <summary>
        /// Maximum fragment size to use for requests
		/// </summary>
		public System.Int32 fragSize;	

		/// <summary>
        /// If true, the master will do time syncs when it sees the time IIN bit from the slave
		/// </summary>
		public System.Boolean allowTimeSync;

		/// <summary>
        /// If true, the master will enable/disable unsol on startup
		/// </summary>
		public System.Boolean doUnsolOnStartup;

		/// <summary>
        /// If DoUnsolOnStartup == true, the master will use this bit to decide wether to enable (true) or disable (false)
		/// </summary>
		public System.Boolean enableUnsol;

		/// <summary>
        /// Bitwise mask used determine which classes are enabled/disabled for unsol
		/// </summary>
		public System.Int32 unsolClassMask;

		/// <summary>
        /// Period for integrity scans (class 0), -1 for non periodic
		/// </summary>
        public TimeSpan integrityPeriod;

		/// <summary>
        /// Time delay between task retries
		/// </summary>
        public TimeSpan taskRetryPeriod;	
	}

    /// <summary>
    /// Structure that records which events are scanned / evented
    /// </summary>
    public struct ClassMask 
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="class1">true for Class1</param>
        /// <param name="class2">true for Class2</param>
        /// <param name="class3">true for Class3</param>
	    public ClassMask(bool class1, bool class2, bool class3)
        {
            this.class1 = class1;
            this.class2 = class2;
            this.class3 = class3;
	    }        

	    public bool class1;
	    public bool class2;
	    public bool class3;	   
    }   

    /// <summary>
    /// Class that defines how many events an outstation database will record before buffer overflow occurs
    /// </summary>
    public class EventMaxConfig {

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="maxBinaryEvents">maximum numner of binary events</param>
        /// <param name="maxAnalogEvents">maximum numner of analog events</param>        
        /// <param name="maxCounterEvents">maximum numner of counter events</param>
        public EventMaxConfig(System.UInt32 maxBinaryEvents, System.UInt32 maxAnalogEvents, System.UInt32 maxCounterEvents)
        {
            this.maxBinaryEvents = maxBinaryEvents;
            this.maxAnalogEvents = maxAnalogEvents;
            this.maxCounterEvents = maxCounterEvents;
        }

        /// <summary>
        /// All events set to 1000
        /// </summary>
        public EventMaxConfig()
        {
            this.maxBinaryEvents = 1000;
            this.maxAnalogEvents = 1000;
            this.maxCounterEvents = 1000;
        }

	    /// <summary>
	    /// The number of binary events the slave will buffer before overflowing */
	    /// </summary>
	    public System.UInt32 maxBinaryEvents;

	    /// <summary>
	    /// The number of analog events the slave will buffer before overflowing
	    /// </summary>
        public System.UInt32 maxAnalogEvents;

	    /// <summary>
	    /// The number of counter events the slave will buffer before overflowing
	    /// </summary>
        public System.UInt32 maxCounterEvents;	
    }


    /// <summary>
    /// Configuration information for a dnp3 slave (outstation)
    /// Used as both input describing the startup configuration of the slave, and as configuration state of mutable properties (i.e. unsolicited responses).
    /// Major feature areas are unsolicited responses, time synchronization requests, event buffer limits, and the DNP3 object/variations to use by default
    /// when the master requests class data or variation 0.
    /// </summary>
    public class SlaveConfig 
    {
        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public SlaveConfig()
        {
            this.maxControls = 1;
            this.disableUnsol = false;
            this.unsolMask = new ClassMask(true, true, true);
            this.allowTimeSync = false;
            this.timeSyncPeriod = TimeSpan.FromMinutes(10);
            this.unsolPackDelay = TimeSpan.FromMilliseconds(200);
            this.unsolRetryDelay = TimeSpan.FromSeconds(2);
            this.selectTimeout = TimeSpan.FromSeconds(5);
            this.maxFragSize = 2048;
            this.eventMaxConfig = new EventMaxConfig();
            this.staticBinary = StaticBinaryResponse.GROUP1_VAR2;
            this.staticAnalog = StaticAnalogResponse.GROUP30_VAR1;
            this.staticCounter = StaticCounterResponse.GROUP20_VAR1;
            this.staticSetpointStatus = StaticSetpointStatusResponse.GROUP40_VAR1;
            this.eventBinary = EventBinaryResponse.GROUP2_VAR1;
            this.eventAnalog = EventAnalogResponse.GROUP32_VAR1;
            this.eventCounter = EventCounterResponse.GROUP22_VAR1;
        }

	    /// <summary>
        /// The maximum number of controls the slave will attempt to process from a single APDU
	    /// </summary>
	    public System.UInt32 maxControls;

	    /// <summary>
        /// if true, fully disables unsolicited mode as if the slave didn't support it
	    /// </summary>
        public bool disableUnsol;

	    /// <summary>
        /// controls what unsol classes are enabled
	    /// </summary>
        public ClassMask unsolMask;

	    /// <summary>
        /// if true, the slave will request time synchronization on an interval
	    /// </summary>
        public bool allowTimeSync;

	    /// <summary>
        /// The period of time sync interval in milliseconds
	    /// </summary>
        public TimeSpan timeSyncPeriod;

	    /// <summary>
        /// The amount of time the slave will wait before sending new unsolicited data (less than 0 == immediate)
	    /// </summary>
        public TimeSpan unsolPackDelay;

	    /// <summary>
        /// How long the slave will wait before retrying an unsuccessful unsol response
	    /// </summary>
        public TimeSpan unsolRetryDelay;

        /// <summary>
        /// How long the outstation will allow an operate to proceed after a prior select
        /// </summary>
        public TimeSpan selectTimeout;

	    /// <summary>
        /// The maximum fragment size the slave will use for data it sends
	    /// </summary>
        public System.UInt32 maxFragSize;	    

	    /// <summary>
        /// Structure that defines the maximum number of events to buffer
	    /// </summary>
        public EventMaxConfig eventMaxConfig;

	    /// <summary>
        /// The default group/variation to use for static binary responses
	    /// </summary>	    
	    public StaticBinaryResponse staticBinary;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
	    public StaticAnalogResponse staticAnalog;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
        public StaticCounterResponse staticCounter;

	    /// <summary>
        /// The default group/variation to use for static setpoint status responses
	    /// </summary>
        public StaticSetpointStatusResponse staticSetpointStatus;

	    /// <summary>
        /// The default group/variation to use for binary event responses
	    /// </summary>
        public EventBinaryResponse eventBinary;

	    /// <summary>
        /// The default group/variation to use for analog event responses
	    /// </summary>
        public EventAnalogResponse eventAnalog;

	    /// <summary>
        /// The default group/variation to use for counter event responses
	    /// </summary>
	    public EventCounterResponse eventCounter;	   	    
    }

    /// <summary>
    /// Base class from which all PointRecords are inherited
    /// </summary>
    public class PointRecord
    {
    
    };

    /// <summary>
    /// Point record type that is assigned an event class 
    /// </summary>
    public class EventPointRecord : PointRecord
    {
        public EventPointRecord(PointClass pointClass) : base()
        {            
            this.pointClass = pointClass;
        }

        public EventPointRecord()
            : this(PointClass.PC_CLASS_0)
        { 
        
        }

        public PointClass pointClass;
    };

    /// <summary>
    /// Point record type that is assigned an event class and deadband tolerance
    /// </summary>
    public class DeadbandEventPointRecord : EventPointRecord
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="pointClass">Class that will be evented when the value changes</param>
        /// <param name="deadband">Value can change within this tolerance without producing an event</param>
        public DeadbandEventPointRecord(PointClass pointClass, double deadband) : base(pointClass)
        {
            this.deadband = deadband;
        }

        /// <summary>
        /// Default constructor with Class 0 and 0.1 tolerance
        /// </summary>
        public DeadbandEventPointRecord()
            : this(PointClass.PC_CLASS_0, 0.1)
        { }

        /// <summary>
        /// Value can change within this tolerance without producing an event
        /// </summary>
        public double deadband;
    };    

    /// <summary>
    /// Defines the database layout for an outstation
    /// </summary>
    public class DeviceTemplate
    {
        /// <summary>
        /// Constructor that sets up the size of the types
        /// </summary>
        /// <param name="numBinary">numer of binary values starting at index 0</param>
        /// <param name="numAnalog">numer of analog values starting at index 0</param>
        /// <param name="numCounter">numer of counter values starting at index 0</param>
        /// <param name="numControlStatus">numer of control status values starting at index 0</param>
        /// <param name="numSetpointStatus">numer of setpoint status values starting at index 0</param>
        public DeviceTemplate(  System.UInt32 numBinary,
                                System.UInt32 numAnalog,
                                System.UInt32 numCounter,
                                System.UInt32 numControlStatus,
                                System.UInt32 numSetpointStatus)
        {
            binaries = Enumerable.Range(0, (int) numBinary).Select(i => new EventPointRecord(PointClass.PC_CLASS_1)).ToList();
            counters = Enumerable.Range(0, (int) numCounter).Select(i => new EventPointRecord(PointClass.PC_CLASS_1)).ToList();
            analogs = Enumerable.Range(0, (int) numAnalog).Select(i => new DeadbandEventPointRecord(PointClass.PC_CLASS_1, 0.0)).ToList();
            controlStatii = Enumerable.Range(0, (int) numControlStatus).Select(i => new PointRecord()).ToList();
            setpointStatii = Enumerable.Range(0, (int) numSetpointStatus).Select(i => new PointRecord()).ToList();            
        }

        /// <summary>
        /// Default constructor that sets up 10 of every type
        /// </summary>
        public DeviceTemplate()
            : this(10, 10, 10, 10, 10)
        { }

        /// <summary>
        /// Modify individual binary configuration here
        /// </summary>
        public List<EventPointRecord> binaries;
        /// <summary>
        /// Modify individual analog configuration here
        /// </summary>
        public List<EventPointRecord> counters;
        /// <summary>
        /// Modify individual counter configuration here
        /// </summary>
        public List<DeadbandEventPointRecord> analogs;
        /// <summary>
        /// Modify individual control status configuration here
        /// </summary>
        public List<PointRecord> controlStatii;
        /// <summary>
        /// Modify individual setpoint status configuration here
        /// </summary>
        public List<PointRecord> setpointStatii;
    };

    /// <summary>
    /// Aggreate configuration for a master stack
    /// </summary>
	public class MasterStackConfig
    {	
        /// <summary>
        /// Reasonable defaults
        /// </summary>
		public MasterStackConfig()
        {
            this.link = new LinkConfig(true, false);
            this.master = new MasterConfig();
            this.app = new AppConfig();
		}        

        /// <summary>
        /// Configuration for a master
        /// </summary>
        public MasterConfig master;
        /// <summary>
        /// Configuration for the application layer
        /// </summary>
        public AppConfig app;
        /// <summary>
        /// Configuration for the link layer
        /// </summary>
        public LinkConfig link;
	}

    /// <summary>
    /// Aggreate configuration for an outstation stack
    /// </summary>
    public class SlaveStackConfig
	{
        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public SlaveStackConfig()
        {
            this.slave = new SlaveConfig();
            this.device = new DeviceTemplate(10, 10, 10, 10, 10);
            this.link = new LinkConfig(false, false);
            this.app = new AppConfig();            
        }

        /// <summary>
        /// Slave config
        /// </summary>
	    public SlaveConfig slave;
        /// <summary>
        /// Device template that specifies database layout, control behavior
        /// </summary>
        public DeviceTemplate device;
        /// <summary>
        /// Application layer config
        /// </summary>
        public AppConfig app;
        /// <summary>
        /// Link layer config
        /// </summary>
        public LinkConfig link;
	}
}
