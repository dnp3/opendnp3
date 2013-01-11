using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    public enum CommandModes {
	    CM_SBO_ONLY,
	    CM_DO_ONLY,	
	    CM_SBO_OR_DO
    }

    public enum CommandTypes {
	    CT_BINARY_OUTPUT,
	    CT_SETPOINT,
	    CT_NONE
    }


    public enum CommandStatus {
	    CS_SUCCESS,			
	    CS_TIMEOUT,			
	    CS_NO_SELECT,		
	    CS_FORMAT_ERROR,	
	    CS_NOT_SUPPORTED,	
	    CS_ALREADY_ACTIVE,	
	    CS_HARDWARE_ERROR,	
	    CS_LOCAL,			
	    CS_TOO_MANY_OPS,	
	    CS_NOT_AUTHORIZED,
	    CS_UNDEFINED
    }

    public enum ControlCode {
	    CC_NULL,
	    CC_PULSE,
	    CC_LATCH_ON,
	    CC_LATCH_OFF,
	    CC_PULSE_CLOSE,
	    CC_PULSE_TRIP,
	    CC_UNDEFINED
    }

    public class BinaryOutput
    {	    
	    public BinaryOutput(ControlCode code, byte count, System.UInt16 onTime, System.UInt16 offTime)
        {
            this.code = code;
            this.count = count;
            this.onTime = onTime;
            this.offTime = offTime;
        }

        public readonly ControlCode code;
        public readonly byte count;
        public readonly System.UInt16 onTime;
        public readonly System.UInt16 offTime;
    }


    public enum SetpointEncodingType {
	    SPET_INT16,			//!< 16bit floating point (dnp Object41var1)
	    SPET_INT32,			//!< 32bit signed integer (dnp Object41var2)
	    SPET_FLOAT,			//!< 32bit floating point (dnp Object41var3)
	    SPET_DOUBLE,		//!< 64bit floating point (dnp Object41var4)
	    SPET_AUTO_INT,		//!< automatically choose smallest valid int type
	    SPET_AUTO_DOUBLE,	//!< automatically choose smallest valid double type
	    SPET_UNSET,			//!< means no type has been guessed or set yet
    }


    public class Setpoint
    {	    	
	    public Setpoint(System.Int16 value)
        {
            this.value = value;
            this.encodingType = SetpointEncodingType.SPET_INT16;
        }
	    
	    public Setpoint(System.Int32 value)
        {
            this.value = value;
            this.encodingType = SetpointEncodingType.SPET_INT32;
        }
	
	    public Setpoint(System.Double value)
        {
            this.value = value;
            this.encodingType = SetpointEncodingType.SPET_DOUBLE;
        }

        public Setpoint(System.Single value)
        {
            this.value = value;
            this.encodingType = SetpointEncodingType.SPET_FLOAT;
        }

	    public readonly double value;
        public SetpointEncodingType encodingType;
    }

}
