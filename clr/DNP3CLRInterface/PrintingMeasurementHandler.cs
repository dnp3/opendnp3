
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
    public class PrintingMeasurementHandler : IMeasurementHandler
    {
        private static readonly IMeasurementHandler instance = new PrintingMeasurementHandler();

        public static IMeasurementHandler Instance
        {
            get
            {
                return instance;
            }
        }

        private PrintingMeasurementHandler()
        { }
              
        public void Load(IMeasurementUpdate update)
        {
            foreach (var v in update.BinaryUpdates) Console.WriteLine("value: " + v.value.value + " index: " + v.index);
            foreach (var v in update.AnalogUpdates) Console.WriteLine("value: " + v.value.value + " index: " + v.index);
            foreach (var v in update.CounterUpdates) Console.WriteLine("value: " + v.value.value + " index: " + v.index);
            foreach (var v in update.ControlStatusUpdates) Console.WriteLine("value: " + v.value.value + " index: " + v.index);
            foreach (var v in update.SetpointStatusUpdates) Console.WriteLine("value: " + v.value.value + " index: " + v.index);
            foreach (var v in update.OctetStringUpdates) Console.WriteLine("value: " + v.value.AsString() + " index: " + v.index);
        }
    }    
}
