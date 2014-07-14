
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
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{

    /// <summary>
    /// Singleton that prints all received values to the console.
    /// </summary>
    public class PrintingSOEHandler : ISOEHandler
    {
        private static readonly ISOEHandler instance = new PrintingSOEHandler();

        public static ISOEHandler Instance
        {
            get
            {
                return instance;
            }
        }

        private PrintingSOEHandler()
        {}

        void ISOEHandler.Start()
        {}

        void ISOEHandler.End()
        {}

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Binary>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Analog>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Counter>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<OctetString>> values)
        {
            Print("Static", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Binary>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Analog>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Counter>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            Print("Event", values);
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<OctetString>> values)
        {
            Print("Event", values);
        }

        private void Print(string header, IEnumerable<IndexedValue<Binary>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " Binary[" + pair.Index + "] " + pair.Value.ToString());             
            }           
        }

        private void Print(string header, IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " DoubleBit[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<Analog>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " Analog[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<Counter>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " Counter[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " FrozenCounter[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " BinaryOutputStatus[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " AnalogOutputStatus[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(string header, IEnumerable<IndexedValue<OctetString>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(header + " OctetString[" + pair.Index + "] lemgth: " + pair.Value.Bytes.Length);
            }
        }
    }    
}
