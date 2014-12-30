
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

        private void PrintHeaderInfo(HeaderInfo info)
        {
            Console.WriteLine(String.Format("{0} : {1} : timestamps {2}", info.variation, info.qualifier, info.tsmode));
        }

        private void Print(IEnumerable<IndexedValue<Binary>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("Binary[" + pair.Index + "] " + pair.Value.ToString());             
            }           
        }

        private void Print(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("DoubleBit[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<Analog>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("Analog[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<Counter>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("Counter[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("FrozenCounter[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("BinaryOutputStatus[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("AnalogOutputStatus[" + pair.Index + "] " + pair.Value.ToString());
            }
        }

        private void Print(IEnumerable<IndexedValue<OctetString>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine("OctetString[" + pair.Index + "] lemgth: " + pair.Value.Bytes.Length);
            }
        }

        private void Print(IEnumerable<IndexedValue<TimeAndInterval>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(String.Format("TimeAndInterval[{0}] {1}", pair.Index, pair.Value));
            }
        }

        private void Print(IEnumerable<IndexedValue<BinaryCommandEvent>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(String.Format("BinaryCommandEvent[{0}] {1}", pair.Index, pair.Value));
            }
        }

        private void Print(IEnumerable<IndexedValue<AnalogCommandEvent>> values)
        {
            foreach (var pair in values)
            {
                Console.WriteLine(String.Format("AnalogCommandEvent[{0}] {1}", pair.Index, pair.Value));
            }
        }


        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<Binary>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<Analog>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<Counter>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<OctetString>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<TimeAndInterval>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<BinaryCommandEvent>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }

        void ISOEHandler.OnReceiveHeader(HeaderInfo info, IEnumerable<IndexedValue<AnalogCommandEvent>> values)
        {
            this.PrintHeaderInfo(info);
            this.Print(values);
        }
    }    
}
