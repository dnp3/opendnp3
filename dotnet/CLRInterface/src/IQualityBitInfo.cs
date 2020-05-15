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
    /// Interface for retrieving descriptions about a specific quality bit
    /// </summary>
    public interface IQualityBitInfo
    {
        /// <summary>
        /// returns a long label
        /// </summary>
        /// <param name="bit">the specific bit [0-7]</param>
        /// <returns>long label representation</returns>
        string GetLongLabel(int bit);

        /// <summary>
        /// returns a short label
        /// </summary>
        /// <param name="bit">the specific bit [0-7]</param>
        /// <returns>short label representation</returns>
        string GetShortLabel(int bit);
    }

    class TypedQualityBitInfo : IQualityBitInfo
    {
        readonly Func<int, String> func;

        public TypedQualityBitInfo(Func<int, String> func)
        { 
            this.func = func;
        }

        public string GetLongLabel(int bit)
        {
            return func(bit);
        }


        string IQualityBitInfo.GetShortLabel(int bit)
        {
            return this.GetLongLabel(bit).Substring(0, 2);
        }
    }

    public static class QualityInfo
    {
        public static readonly IQualityBitInfo binary = new TypedQualityBitInfo(GetBinaryString);
        public static readonly IQualityBitInfo doubleBinary = new TypedQualityBitInfo(GetDoubleBitBinaryString);
        public static readonly IQualityBitInfo counter = new TypedQualityBitInfo(GetCounterString);
        public static readonly IQualityBitInfo analog = new TypedQualityBitInfo(GetAnalogString);
        public static readonly IQualityBitInfo binaryOutputStatus = new TypedQualityBitInfo(GetBinaryOutputStatusString);
        public static readonly IQualityBitInfo octetString = new TypedQualityBitInfo(x => "");

        public static int CountOf(byte flags)
        {
            var count = 0;

            for (byte i = 0; i < 8; ++i)
            {
                if ((flags & (1 << i)) != 0)
                {
                    ++count;
                }
            }

            return count;
        }

        public static IEnumerable<string> GetLongNames(byte flags, IQualityBitInfo info)
        {
            for (byte i = 0; i < 8; ++i)
            {
                if ((flags & (1 << i)) != 0)
                {
                    yield return info.GetLongLabel(i);
                }
            }
        }

        public static IEnumerable<string> GetShortNames(byte flags, IQualityBitInfo info)
        {
            for (byte i = 0; i < 8; ++i)
            {
                if ((flags & (1 << i)) != 0)
                {
                    yield return info.GetShortLabel(i);
                }
            }
        }
        
        static string GetBinaryString(int index)
        {
            return ((BinaryQuality) (1 << index)).ToString();
        }

        static string GetDoubleBitBinaryString(int index)
        {
            return ((DoubleBitBinaryQuality) (1 << index)).ToString();
        }

        static string GetCounterString(int index)
        {
            return ((CounterQuality)(1 << index)).ToString();
        }       

        static string GetAnalogString(int index)
        {
            return ((AnalogQuality)(1 << index)).ToString();
        }

        static string GetBinaryOutputStatusString(int index)
        {
            return ((BinaryOutputStatusQuality)(1 << index)).ToString();
        }
        
    }
}
