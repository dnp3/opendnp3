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
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Configuration information for the dnp3 master
    /// </summary>
    [Serializable]
    public class MasterConfig
    {
        public MasterConfig()
        {

        }

        /// <summary>
        /// If true, the master will do time syncs when it sees the time IIN bit from the outstation
        /// </summary>
        public TimeSyncMode timeSyncMode = TimeSyncMode.None;

        /// <summary>
        /// If true, the master will disable unsol on startup for all 3 classes
        /// </summary>
        public bool disableUnsolOnStartup = true;

        /// <summary>
        /// Bitwise mask used determine which classes are enabled for unsol, if 0 unsol is not enabled
        /// </summary>
        public ClassField unsolClassMask = ClassField.AllEventClasses;

        /// <summary>
        /// Which classes should be requested in a startup integrity scan, defaults to 3/2/1/0
        /// A mask equal to 0 means no startup integrity scan will be performed
        /// </summary>
        public ClassField startupIntegrityClassMask = ClassField.AllClasses;

        /// <summary>
        /// Defines whether an integrity scan will be performed when the EventBufferOverflow IIN is detected
        /// </summary>
        public bool integrityOnEventOverflowIIN = true;

        /// <summary>
        /// Which classes should be requested in an event scan when detecting corresponding events available IIN
        /// </summary>
        public ClassField eventScanOnEventsAvailableClassMask = ClassField.None;

        /// <summary>
        /// Control how the master chooses what qualifier to send when making requests.
        /// The default behavior is to always use two bytes, but the one byte optimization can be enabled.        
        /// </summary>
        public IndexQualifierMode controlQualifierMode = IndexQualifierMode.always_two_bytes;

        /// <summary>
        /// Application layer response timeout
        /// </summary>
        [XmlIgnore]
        public TimeSpan responseTimeout = TimeSpan.FromSeconds(5);

        /// <summary>
        /// Time delay beforce retrying a failed task
        /// </summary>
        [XmlIgnore]
        public TimeSpan taskRetryPeriod = TimeSpan.FromSeconds(5);

        /// <summary>
        /// Time delay beforce retrying a failed task
        /// </summary>
        [XmlIgnore]
        public TimeSpan taskStartTimeout = TimeSpan.FromSeconds(10);


        [XmlElement]
        public long TaskResponseTimeoutMilliseconds
        {
            get
            {
                return (responseTimeout.Ticks / TimeSpan.TicksPerMillisecond);
            }
            set
            {
                responseTimeout = TimeSpan.FromMilliseconds(value);
            }
        }


        [XmlElement]
        public long TaskRetryPeriodMilliseconds
        {
            get
            {
                return (taskRetryPeriod.Ticks / TimeSpan.TicksPerMillisecond);
            }
            set
            {
                taskRetryPeriod = TimeSpan.FromMilliseconds(value);
            }
        }

        [XmlElement]
        public long TaskStartTimeoutMilliseconds
        {
            get
            {
                return (taskStartTimeout.Ticks / TimeSpan.TicksPerMillisecond);
            }
            set
            {
                taskStartTimeout = TimeSpan.FromMilliseconds(value);
            }
        }
    }

}
