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

namespace Automatak.DNP3.Interface
{
    public class DNPTime
    {
        public static readonly DNPTime Unset = new DNPTime(DateTime.MinValue, TimestampQuality.INVALID);
        private static readonly DateTime Epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);

        public static DNPTime Now
        {
            get { return new DNPTime(DateTime.Now, TimestampQuality.SYNCHRONIZED); }
        }

        public static DNPTime FromEpoch(Int64 epochTime, TimestampQuality quality)
        {
            var ms = TimeSpan.FromMilliseconds(epochTime);
            if (ms > DateTime.MaxValue - Epoch)
            {
                return new DNPTime(DateTime.MaxValue, quality);
            }
            else
            {
                return new DNPTime(Epoch.Add(ms), quality);
            }
        }

        public DNPTime(DateTime time) : this(time, TimestampQuality.SYNCHRONIZED)
        {
        }

        public DNPTime(DateTime time, TimestampQuality quality)
        {
            this.value = time;
            this.quality = quality;
        }

        public DateTime Value
        {
            get { return this.value; }
        }

        public TimestampQuality Quality
        {
            get { return this.quality; }
        }

        public UInt64 ToEpoch()
        {
            var ticks = (UInt64)this.value.ToUniversalTime().Subtract(Epoch).Ticks;
            return ticks / TimeSpan.TicksPerMillisecond;
        }

        public override string ToString()
        {
            return this.value.ToUniversalTime().ToString() + " [" + this.quality.ToString() +"]";
        }

        private DateTime value;
        private TimestampQuality quality;
    }
}
