using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;
using TestSetControlLibrary;
using System.ComponentModel;

namespace DotNetTestSet
{
    class SynchronizingMeasurementSource : IMeasurementSource
    {
        ISynchronizeInvoke sync;

        public SynchronizingMeasurementSource(ISynchronizeInvoke sync)
        {
            this.sync = sync;
        }


        public void Update(Binary meas, UInt32 index, String id)
        {
            if (this.AllBinaryUpdate != null) sync.BeginInvoke(new Action(() => AllBinaryUpdate(meas, index, id)), null);
        }

        public void Update(Analog meas, UInt32 index, String id)
        {
            if (this.AllAnalogUpdate != null) sync.BeginInvoke(new Action(() => AllAnalogUpdate(meas, index, id)), null);
        }

        public void Update(Counter meas, UInt32 index, String id)
        {
            if (this.AllCounterUpdate != null) sync.BeginInvoke(new Action(() => AllCounterUpdate(meas, index, id)), null);
        }

        public void Update(ControlStatus meas, UInt32 index, String id)
        {
            if (this.AllControlStatusUpdate != null) sync.BeginInvoke(new Action(() => AllControlStatusUpdate(meas, index, id)), null);
        }

        public void Update(SetpointStatus meas, UInt32 index, String id)
        {
            if (this.AllSetpointStatusUpdate != null) sync.BeginInvoke(new Action(() => AllSetpointStatusUpdate(meas, index, id)), null);
        }


        #region IMeasurementSource Members

        public event OnUpdateBinary AllBinaryUpdate;

        public event OnUpdateAnalog AllAnalogUpdate;

        public event OnUpdateCounter AllCounterUpdate;

        public event OnUpdateControlStatus AllControlStatusUpdate;

        public event OnUpdateSetpointStatus AllSetpointStatusUpdate;

        #endregion
    }

    /// <summary>
    /// Adapts IDataObserver to a event type interface
    /// </summary>
    class EventedDataObserver : IDataObserver
    {
        private readonly SynchronizingMeasurementSource source;

        public EventedDataObserver(ISynchronizeInvoke sync)
        {
            source = new SynchronizingMeasurementSource(sync);
        }

        public IMeasurementSource MeasurementSource
        {
            get 
            {
                return source;
            }
        }

        public void Start()
        {
            
        }

        public void Update(Binary update, uint index)
        {
            source.Update(update, index, "");
        }

        public void Update(Analog update, uint index)
        {
            source.Update(update, index, "");
        }

        public void Update(Counter update, uint index)
        {
            source.Update(update, index, "");
        }

        public void Update(ControlStatus update, uint index)
        {
            source.Update(update, index, "");
        }

        public void Update(SetpointStatus update, uint index)
        {
            source.Update(update, index, "");
        }

        public void End()
        {

        }
    }
}
