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

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Represent a start-stop range
    /// </summary>
    public struct Range
    {        
        public static Range From(UInt16 start, UInt16 stop)
        {
            return new Range(start, stop);
        }

        public static Range All()
        {
            return new Range(0, UInt16.MaxValue);
        }

        public bool IsDefined()
        {
            return start <= stop;
        }

        public bool IsAllObjects()
        {
            return start == 0 && stop == UInt16.MaxValue;
        }

        Range(UInt16 start, UInt16 stop)
        {
            this.start = start;
            this.stop = stop;
        }

        public UInt16 start;
        public UInt16 stop;     
    }

    public struct ClassAssignment
    {
        public ClassAssignment(byte group, byte variation, PointClass clazz, Range range)
        {
            this.group = group;
            this.variation = variation;
            this.clazz = clazz;
            this.range = range;
        }

        public byte group;
        public byte variation;
        public PointClass clazz;
        
        // if range is full, this implies 0x06
        public Range range;
    }
}
