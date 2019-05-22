// Copyright 2013-2019 Automatak, LLC
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
    /// <summary>
    /// Collection of static methods for translating dnp3 to .NET timestamps and back
    /// </summary>
    public class TimeStamp
    {
        /// <summary>
        /// dnp3 uses a different epoch than .NET
        /// </summary>
        private static readonly DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, System.DateTimeKind.Utc);

        /// <summary>
        /// Maximum timestamp value that can be added to the epoch
        /// </summary>
        private static readonly TimeSpan max = DateTime.MaxValue - epoch;
       
        /// <summary>
        /// Converts a dnp3 timestamp to a .NET DateTime
        /// </summary>
        /// <param name="time">milliseconds since unix epoch</param>
        /// <returns>.NET DateTime</returns>
        public static DateTime Convert(Int64 time)        
	    {
            var ms = TimeSpan.FromMilliseconds(time);
            if(ms > max)
            {
                return DateTime.MaxValue;                       
            }
            else
            {
                return epoch.Add(ms);    
            }                       
	    }

        /// <summary>
        /// Converts a .NET DateTime object to a dnp3 timestamp
        /// </summary>
        /// <param name="time">DateTime object</param>
        /// <returns>dnp3 milliseconds since unix epoch</returns>
        public static Int64 Convert(DateTime time)
	    {
            var ticks = time.ToUniversalTime().Subtract(epoch).Ticks;
            return (ticks / TimeSpan.TicksPerMillisecond);
        }        
    }
}
