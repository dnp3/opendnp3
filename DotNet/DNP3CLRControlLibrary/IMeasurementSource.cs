using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public delegate void OnUpdateBinary(Binary meas, UInt32 index, String id);
    public delegate void OnUpdateAnalog(Analog meas, UInt32 index, String id);
    public delegate void OnUpdateCounter(Counter meas, UInt32 index, String id);
    public delegate void OnUpdateControlStatus(ControlStatus meas, UInt32 index, String id);
    public delegate void OnUpdateSetpointStatus(SetpointStatus meas, UInt32 index, String id);

    public interface IMeasurementSource
    {        
        event OnUpdateBinary AllBinaryUpdate;
        event OnUpdateAnalog AllAnalogUpdate;
        event OnUpdateCounter AllCounterUpdate;
        event OnUpdateControlStatus AllControlStatusUpdate;
        event OnUpdateSetpointStatus AllSetpointStatusUpdate;
    }
}
