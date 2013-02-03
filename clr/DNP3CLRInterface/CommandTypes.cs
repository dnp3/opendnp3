
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{   
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

    public class ControlRelayOutputBlock
    {
        public ControlRelayOutputBlock(ControlCode code, byte count, System.UInt16 onTime, System.UInt16 offTime)
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


   
    public class AnalogOutput<T>
    {	    	
	    public AnalogOutput(T value)
        {
            this.value = value;            
        }	   	    

	    public readonly T value;        
    }

    public class AnalogOutputInt32 : AnalogOutput<System.Int32>
    {
        public AnalogOutputInt32(System.Int32 value) : base(value)
        {}
    }

    public class AnalogOutputInt16 : AnalogOutput<System.Int16>
    {
        public AnalogOutputInt16(System.Int16 value)
            : base(value)
        { }
    }

    public class AnalogOutputFloat32 : AnalogOutput<float>
    {
        public AnalogOutputFloat32(float value)
            : base(value)
        { }
    }

    public class AnalogOutputDouble64 : AnalogOutput<double>
    {
        public AnalogOutputDouble64(System.Double value)
            : base(value)
        { }
    }

}
