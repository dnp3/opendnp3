using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    public class LinkConfig
	{
		
		public LinkConfig(	bool isMaster, 
						    bool useConfirms, 
						    System.UInt32 numRetry, 
						    System.UInt16 localAddr, 
						    System.UInt16 remoteAddr, 
						    System.UInt64 timeout)							
		{
            this.isMaster = isMaster;
            this.useConfirms = useConfirms;
            this.numRetry = numRetry;
            this.localAddr = localAddr;
            this.remoteAddr = remoteAddr;
            this.timeout = timeout;
        }

		public LinkConfig(bool isMaster, bool useConfirms)
        {
			this.isMaster = isMaster;
			this.useConfirms = useConfirms;
			this.numRetry = 0;
			this.localAddr = (ushort) (isMaster ? 1 : 1024);
			this.remoteAddr = (ushort) (isMaster ? 1024 : 1);
			this.timeout = 1000;
		}


        public LinkConfig(): this(true, false)
        {
            
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
		/// the response timeout in milliseconds for confirmed requests
		/// </summary>
		public System.UInt64 timeout;
	}

	public class AppConfig
	{		
		public AppConfig()
        {
            this.rspTimeout = 5000;
            this.numRetry = 0;
            this.fragSize = 2048;
        }

		public AppConfig(System.Int64 rspTimeout, System.Int32 numRetry, System.Int32 fragSize)
        {
			this.rspTimeout = rspTimeout;
			this.numRetry = numRetry;
			this.fragSize = fragSize;
        }
		

		/// <summary>
		/// The response/confirm timeout in millisec
		/// </summary>
		public System.Int64 rspTimeout;

		/// <summary>
		/// Number of retries performed for applicable frames
		/// </summary>
        public System.Int32 numRetry;

		/// <summary>
		/// The maximum size of received application layer fragments
		/// </summary>
        public System.Int32 fragSize;
	}
	
	public class ExceptionScan {
		
        public ExceptionScan(System.Int32 classMask, System.Int64 scanRateMs)
        {
		    this.classMask = classMask;
            this.scanRateMs = scanRateMs;
	    }

        public ExceptionScan() 
        {
            this.classMask = (Int32) PointClass.PC_CLASS_0;
            this.scanRateMs = 5000;
        }

        public System.Int32 classMask;
        public System.Int64 scanRateMs;

	};
	
	public enum PointClass {
		PC_CLASS_0 = 0x01,
		PC_CLASS_1 = 0x02,
		PC_CLASS_2 = 0x04,
		PC_CLASS_3 = 0x08,
		PC_ALL_EVENTS = PC_CLASS_1 | PC_CLASS_2 | PC_CLASS_3,
        PC_INVALID = 0x10
	}

	/// Configuration information for the dnp3 master
	public class MasterConfig {
				
		public MasterConfig()
        {
			fragSize = 2048;
			allowTimeSync = true;
			doUnsolOnStartup = false;
			enableUnsol = true;
			unsolClassMask = (System.Int32) (PointClass.PC_ALL_EVENTS);
			integrityRate = 5000;
			taskRetryRate = 5000;
			scans = new List<ExceptionScan>();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="classMask"></param>
		/// <param name="period"></param>
		public void AddExceptionScan(System.Int32 classMask, System.Int64 period)
        {
			scans.Add(new ExceptionScan(classMask, period));			
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
		public System.Int64 integrityRate;

		/// <summary>
        /// Time delay between task retries
		/// </summary>
		public System.Int64 taskRetryRate;

		/// <summary>
        /// vector that holds exception scans
		/// </summary>
		public List<ExceptionScan> scans;
	}

    public struct ClassMask 
    {
	    public ClassMask(bool c1, bool c2, bool c3)
        {
            this.class1 = c1;
            this.class2 = c2;
            this.class3 = c3;
	    }        

	    public bool class1;
	    public bool class2;
	    public bool class3;	   
    }

    // Group/Variation pair
    public struct GrpVar {	    
	    
        public GrpVar(int grp, int var)
        {
            this.grp = grp;
            this.var = var;
        }        

	    public int grp;
        public int var;
    }

    public class EventMaxConfig {

        public EventMaxConfig(System.UInt32 maxBinaryEvents, System.UInt32 maxAnalogEvents, System.UInt32 maxCounterEvents)
        {
            this.maxBinaryEvents = maxBinaryEvents;
            this.maxAnalogEvents = maxAnalogEvents;
            this.maxCounterEvents = maxCounterEvents;
        }

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
        public SlaveConfig()
        {
            this.maxControls = 1;
            this.disableUnsol = false;
            this.unsolMask = new ClassMask(true, true, true);
            this.allowTimeSync = false;
            this.timeSyncPeriod = 10 * 60 * 1000; // every 10 min
            this.unsolPackDelay = 200;
            this.unsolRetryDelay = 2000;
            this.maxFragSize = 2048;
            this.eventMaxConfig = new EventMaxConfig();
            this.staticBinary = new GrpVar(1, 2);
            this.staticAnalog = new GrpVar(30, 1);
            this.staticCounter = new GrpVar(20, 1);
	        this.staticSetpointStatus = new GrpVar(40, 1);
	        this.eventBinary = new GrpVar(2, 1);
	        this.eventAnalog = new GrpVar(32, 1);
	        this.eventCounter = new GrpVar(22, 1);
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
        public System.Int64 timeSyncPeriod;

	    /// <summary>
        /// The amount of time the slave will wait before sending new unsolicited data ( <= 0 == immediate)
	    /// </summary>
        public System.Int64 unsolPackDelay;

	    /// <summary>
        /// How long the slave will wait before retrying an unsuccessful unsol response
	    /// </summary>
        public System.Int64 unsolRetryDelay;


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
	    public GrpVar staticBinary;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
	    public GrpVar staticAnalog;

	    /// <summary>
        /// The default group/variation to use for static analog responses
	    /// </summary>
        public GrpVar staticCounter;

	    /// <summary>
        /// The default group/variation to use for static setpoint status responses
	    /// </summary>
        public GrpVar staticSetpointStatus;

	    /// <summary>
        /// The default group/variation to use for binary event responses
	    /// </summary>
        public GrpVar eventBinary;

	    /// <summary>
        /// The default group/variation to use for analog event responses
	    /// </summary>
        public GrpVar eventAnalog;

	    /// <summary>
        /// The default group/variation to use for counter event responses
	    /// </summary>
	    public GrpVar eventCounter;	   	    
    }

    public class PointRecord
    {
    
    };

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

    public class DeadbandEventPointRecord : EventPointRecord
    {
        public DeadbandEventPointRecord(PointClass pointClass, double deadband) : base(pointClass)
        {
            this.deadband = deadband;
        }

        public DeadbandEventPointRecord()
            : this(PointClass.PC_CLASS_0, 0.1)
        { }

        public double deadband;
    };

    public class ControlRecord : PointRecord
    {
        public ControlRecord(CommandModes mode, System.Int64 selectTimeoutMs) : base()
        {
            this.mode = mode;
            this.selectTimeoutMs = selectTimeoutMs;
        }

        public ControlRecord()
            : this(CommandModes.CM_SBO_ONLY, 5000)
        { }

        public CommandModes mode;
        public System.Int64 selectTimeoutMs;
    };

    public class DeviceTemplate
    {
        public DeviceTemplate(  System.UInt32 numBinary,
                                System.UInt32 numAnalog,
                                System.UInt32 numCounter,
                                System.UInt32 numControlStatus,
                                System.UInt32 numSetpointStatus,
                                System.UInt32 numControls,
                                System.UInt32 numSetpoints)
        {
            binaries = Enumerable.Range(0, (int) numBinary).Select(i => new EventPointRecord(PointClass.PC_CLASS_0)).ToList();
            counters = Enumerable.Range(0, (int) numCounter).Select(i => new EventPointRecord(PointClass.PC_CLASS_0)).ToList();
            analogs = Enumerable.Range(0, (int) numAnalog).Select(i => new DeadbandEventPointRecord(PointClass.PC_CLASS_0, 0.0)).ToList();
            controlStatii = Enumerable.Range(0, (int) numControlStatus).Select(i => new PointRecord()).ToList();
            setpointStatii = Enumerable.Range(0, (int) numSetpointStatus).Select(i => new PointRecord()).ToList();
            controls = Enumerable.Range(0, (int) numControls).Select(i => new ControlRecord(CommandModes.CM_SBO_ONLY, 5000)).ToList();
            setpoints = Enumerable.Range(0, (int )numSetpoints).Select(i => new ControlRecord(CommandModes.CM_SBO_ONLY, 5000)).ToList();
        }

        public DeviceTemplate()
            : this(10, 10, 10, 10, 10, 10, 10)
        { }

        public List<EventPointRecord> binaries;
        public List<EventPointRecord> counters;
        public List<DeadbandEventPointRecord> analogs;
        public List<PointRecord> controlStatii;
        public List<PointRecord> setpointStatii;

        public List<ControlRecord> controls;
        public List<ControlRecord> setpoints;
    };

	public class MasterStackConfig
    {	
		public MasterStackConfig()
        {
            this.link = new LinkConfig(true, false);
            this.master = new MasterConfig();
            this.app = new AppConfig();
		}        

        public MasterConfig master;
        public AppConfig app;
        public LinkConfig link;
	}

    public class SlaveStackConfig
	{
        public SlaveStackConfig()
        {
            this.slave = new SlaveConfig();
            this.device = new DeviceTemplate(10, 10, 10, 10, 10, 10, 10);
            this.link = new LinkConfig(false, false);
            this.app = new AppConfig();
            
        }

	    public SlaveConfig slave;		// Slave config
	    public DeviceTemplate device;	// Device template that specifies database layout, control behavior
        public AppConfig app;			// Application layer config
        public LinkConfig link;		    // Link layer config
	}
}
