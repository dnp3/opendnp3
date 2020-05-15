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

namespace Automatak.DNP3.Interface
{
    public class Flags
    {
        public Flags()
        {
            this.value = 0;
        }

        public Flags(byte value)
        {
            this.value = value;
        }

        public byte Value
        {
            get { return this.value; }
        }

        public bool ToBinaryValue()
        {
            return IsSet(BinaryQuality.STATE);
        }

        public DoubleBit ToDoubleBitValue()
        {
            byte value = (byte)((this.value >> 6) & 0xFF);
            return (DoubleBit)value;
        }

        public bool IsSet(BinaryQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(DoubleBitBinaryQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(AnalogQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(CounterQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(FrozenCounterQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(BinaryOutputStatusQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public bool IsSet(AnalogOutputStatusQuality flag)
        {
            return (this.value & (byte)flag) != 0;
        }

        public void Set(BinaryQuality flag)
        {
            this.value |= (byte)flag;
        }

        public void Set(DoubleBitBinaryQuality flag)
        {
            this.value |= (byte)flag;
        }

        public void Set(AnalogQuality flag)
        {
            this.value |= (byte)flag;
        }

        public void Set(CounterQuality flag)
        {
            this.value |= (byte)flag;
        }

        public void Set(FrozenCounterQuality flag)
        {
            this.value |= (byte)flag;
        }

        public void Set(BinaryOutputStatusQuality flag)
        {
            this.value |= (byte)flag;
        }
        public void Set(AnalogOutputStatusQuality flag)
        {
            this.value |= (byte)flag;
        }

        private byte value;
    }
}
