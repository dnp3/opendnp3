
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

namespace DNP3.Interface
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

        void ISOEHandler.LoadStatic(Binary meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(DoubleBitBinary meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(Analog meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(Counter meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(FrozenCounter meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(BinaryOutputStatus meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(AnalogOutputStatus meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadStatic(OctetString meas, UInt16 index)
        {
            Print("Static", meas, index);
        }

        void ISOEHandler.LoadEvent(Binary meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(DoubleBitBinary meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(Analog meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(Counter meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(FrozenCounter meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(BinaryOutputStatus meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(AnalogOutputStatus meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        void ISOEHandler.LoadEvent(OctetString meas, UInt16 index)
        {
            Print("Event", meas, index);
        }

        private void Print(string header, Binary meas, UInt16 index)
        {
           Console.WriteLine(header + " Binary[" + index + "] " + meas.ToString());            
        }

        private void Print(string header, DoubleBitBinary meas, UInt16 index)
        {
            Console.WriteLine(header + " DoubleBit[" + index + "] " + meas.ToString());                        
        }

        private void Print(string header, Analog meas, UInt16 index)
        {
            Console.WriteLine(header + " Analog[" + index + "] " + meas.ToString()); 
        }

        private void Print(string header, Counter meas, UInt16 index)
        {
            Console.WriteLine(header + " Counter[" + index + "] " + meas.ToString());            
        }

        private void Print(string header, FrozenCounter meas, UInt16 index)
        {
            Console.WriteLine(header + " FrozenCounter[" + index + "] " + meas.ToString());            
        }

        private void Print(string header, BinaryOutputStatus meas, UInt16 index)
        {
            Console.WriteLine(header + " BinaryOutputStatus[" + index + "] " + meas.ToString());            
        }

        private void Print(string header, AnalogOutputStatus meas, UInt16 index)
        {
            Console.WriteLine(header + " AnalogOutputStatus[" + index + "] " + meas.ToString());            
        }

        private void Print(string header, OctetString meas, UInt16 index)
        {
            Console.WriteLine(header + " OctetString[" + index + "] lemgth: " + meas.Bytes.Length);
        }
    }    
}
