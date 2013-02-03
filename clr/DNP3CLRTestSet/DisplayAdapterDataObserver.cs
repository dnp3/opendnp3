
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
