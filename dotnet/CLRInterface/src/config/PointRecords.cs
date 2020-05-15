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

namespace Automatak.DNP3.Interface
{     
    /// <summary>
    /// All types are assigned to a class
    /// </summary>
    public class EventConfig
    {      
        public EventConfig()            
        {
            this.clazz = PointClass.Class1;         
        }        

        public PointClass clazz;        
    };

    /// <summary>
    /// Config type that is assigned an event class and deadband tolerance
    /// </summary>
    public class DeadbandConfig<T> : EventConfig
    {
        public DeadbandConfig() : base()
        {
            this.deadband = default(T);
        }      

        /// <summary>
        /// Value can change within this tolerance without producing an event
        /// </summary>
        public T deadband;
    };

    public class BinaryConfig : EventConfig
    {
        public BinaryConfig() : base()
        {
            this.eventVariation = EventBinaryVariation.Group2Var1;
            this.staticVariation = StaticBinaryVariation.Group1Var2;         
        }

        public EventBinaryVariation eventVariation;
        public StaticBinaryVariation staticVariation;        
    };

    public class DoubleBinaryConfig : EventConfig
    {
        public DoubleBinaryConfig() : base()
        {
            this.eventVariation = EventDoubleBinaryVariation.Group4Var1;
            this.staticVariation = StaticDoubleBinaryVariation.Group3Var2;
        }

        public EventDoubleBinaryVariation eventVariation;
        public StaticDoubleBinaryVariation staticVariation;
    };

    public class BinaryOutputStatusConfig : EventConfig
    {
        public BinaryOutputStatusConfig() : base()
        {
            this.eventVariation = EventBinaryOutputStatusVariation.Group11Var1;
            this.staticVariation = StaticBinaryOutputStatusVariation.Group10Var2;
        }

        public EventBinaryOutputStatusVariation eventVariation;
        public StaticBinaryOutputStatusVariation staticVariation;
    };

    public class CounterConfig : DeadbandConfig<System.UInt32>
    {
        public CounterConfig() : base()
        {
            this.eventVariation = EventCounterVariation.Group22Var1;
            this.staticVariation = StaticCounterVariation.Group20Var1;
        }

        public EventCounterVariation eventVariation;
        public StaticCounterVariation staticVariation;
    };

    public class FrozenCounterConfig : DeadbandConfig<System.UInt32>
    {
        public FrozenCounterConfig() : base()
        {
            this.eventVariation = EventFrozenCounterVariation.Group23Var1;
            this.staticVariation = StaticFrozenCounterVariation.Group21Var1;
        }

        public EventFrozenCounterVariation eventVariation;
        public StaticFrozenCounterVariation staticVariation;
    };

    public class AnalogConfig : DeadbandConfig<double>
    {
        public AnalogConfig() : base()
        {
            this.eventVariation = EventAnalogVariation.Group32Var1;
            this.staticVariation = StaticAnalogVariation.Group30Var1;
        }

        public EventAnalogVariation eventVariation;
        public StaticAnalogVariation staticVariation;
    };

    public class AnalogOutputStatusConfig : DeadbandConfig<double>
    {
        public AnalogOutputStatusConfig() : base()
        {
            this.eventVariation = EventAnalogOutputStatusVariation.Group42Var1;
            this.staticVariation = StaticAnalogOutputStatusVariation.Group40Var1;
        }

        public EventAnalogOutputStatusVariation eventVariation;
        public StaticAnalogOutputStatusVariation staticVariation;
    };

    public class TimeAndIntervalConfig
    {
        public TimeAndIntervalConfig()
        {
            this.staticVariation = StaticTimeAndIntervalVariation.Group50Var4;
        }

        public StaticTimeAndIntervalVariation staticVariation;
    };

    public class OctetStringConfig : EventConfig
    {
        public OctetStringConfig() : base()
        {
            this.eventVariation = EventOctetStringVariation.Group111Var0;
            this.staticVariation = StaticOctetStringVariation.Group110Var0;
        }

        public EventOctetStringVariation eventVariation;
        public StaticOctetStringVariation staticVariation;
    };
}
