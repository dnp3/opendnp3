
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
    public class IndexedValue<T>
    {
        public IndexedValue(T value, System.UInt32 index)
        {
            this.value = value;
            this.index = index;
        }

        public readonly T value;
        public readonly System.UInt32 index;
    }
    
    /// <summary>
    /// An abstract measurement update
    /// </summary>
    public interface IMeasurementUpdate
    {
        IReadOnlyCollection<IndexedValue<Binary>> BinaryUpdates { get; }
        IReadOnlyCollection<IndexedValue<Analog>> AnalogUpdates { get; }
        IReadOnlyCollection<IndexedValue<Counter>> CounterUpdates { get; }
        IReadOnlyCollection<IndexedValue<BinaryOutputStatus>> BinaryOutputStatusUpdates { get; }
        IReadOnlyCollection<IndexedValue<AnalogOutputStatus>> AnalogOutputStatusUpdates { get; }
        IReadOnlyCollection<IndexedValue<OctetString>> OctetStringUpdates { get; }
    }

    public class MeasurementUpdate : IMeasurementUpdate
    {
        public void Load(Binary v, UInt32 index) { this.binaryUpdates.Add(new IndexedValue<Binary>(v, index)); }
        public void Load(Analog v, UInt32 index) { this.analogUpdates.Add(new IndexedValue<Analog>(v, index)); }
        public void Load(Counter v, UInt32 index) { this.counterUpdates.Add(new IndexedValue<Counter>(v, index)); }
        public void Load(BinaryOutputStatus v, UInt32 index) { this.binaryOutputStatusUpdates.Add(new IndexedValue<BinaryOutputStatus>(v, index)); }
        public void Load(AnalogOutputStatus v, UInt32 index) { this.analogOutputStatusUpdates.Add(new IndexedValue<AnalogOutputStatus>(v, index)); }
        public void Load(OctetString v, UInt32 index) { this.octetStringUpdates.Add(new IndexedValue<OctetString>(v, index)); }


        public IReadOnlyCollection<IndexedValue<Binary>> BinaryUpdates
        {
            get { return binaryUpdates; }
        }

        public IReadOnlyCollection<IndexedValue<Analog>> AnalogUpdates
        {
            get { return analogUpdates; }
        }

        public IReadOnlyCollection<IndexedValue<Counter>> CounterUpdates
        {
            get { return counterUpdates; }
        }

        public IReadOnlyCollection<IndexedValue<BinaryOutputStatus>> BinaryOutputStatusUpdates
        {
            get { return BinaryOutputStatusUpdates; }
        }

        public IReadOnlyCollection<IndexedValue<AnalogOutputStatus>> AnalogOutputStatusUpdates
        {
            get { return AnalogOutputStatusUpdates; }
        }

        public IReadOnlyCollection<IndexedValue<OctetString>> OctetStringUpdates
        {
            get { return octetStringUpdates; }
        }

        private readonly List<IndexedValue<Binary>> binaryUpdates = new List<IndexedValue<Binary>>();
        private readonly List<IndexedValue<Analog>> analogUpdates = new List<IndexedValue<Analog>>();
        private readonly List<IndexedValue<Counter>> counterUpdates = new List<IndexedValue<Counter>>();
        private readonly List<IndexedValue<BinaryOutputStatus>> binaryOutputStatusUpdates = new List<IndexedValue<BinaryOutputStatus>>();
        private readonly List<IndexedValue<AnalogOutputStatus>> analogOutputStatusUpdates = new List<IndexedValue<AnalogOutputStatus>>();
        private readonly List<IndexedValue<OctetString>> octetStringUpdates = new List<IndexedValue<OctetString>>();
    }

    /// <summary>
    /// Interface called to receive measurement callbacks from the master
    /// </summary>
    public interface IMeasurementHandler
	{
        void Load(IMeasurementUpdate update);
	}
}
