
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

using System.Text;

namespace DNP3.Interface
{
    public static class LogLevels
    {
        public static readonly System.UInt32 NORMAL = LogFilters.EVENT | LogFilters.ERROR | LogFilters.WARNING | LogFilters.INFO;
        public static readonly System.UInt32 NONE = 0;
        public static readonly System.UInt32 ALL = ~NONE;
    }

    /// <summary>
    /// Constant log filter bitfield
    /// </summary>
    public static class LogFilters
    {
        public static readonly System.UInt32 EVENT = 1;
        public static readonly System.UInt32 ERROR = 1 << 1;
        public static readonly System.UInt32 WARNING = 1 << 2;
        public static readonly System.UInt32 INFO = 1 << 3;
        public static readonly System.UInt32 DEBUG = 1 << 4;
        public static readonly System.UInt32 INTERPRET = 1 << 5;
        public static readonly System.UInt32 COMM = 1 << 6;

        public static string GetFilterString(System.UInt32 filters)
        {
            var builder = new StringBuilder();
            OptionallyAppend(builder, filters, EVENT, "V");
            OptionallyAppend(builder, filters, ERROR, "E");
            OptionallyAppend(builder, filters, WARNING, "W");
            OptionallyAppend(builder, filters, INFO, "I");
            OptionallyAppend(builder, filters, DEBUG, "D");
            OptionallyAppend(builder, filters, INTERPRET, "P");
            OptionallyAppend(builder, filters, COMM, "C");
            return builder.ToString();
        }

        static void OptionallyAppend(StringBuilder builder, System.UInt32 filters, System.UInt32 flag, string s)
        {
            if ((filters & flag) != 0)
            {
                var notFirst = builder.Length > 0;
                if (notFirst)
                {
                    builder.Append("/");
                }
                builder.Append(s);
            }
        }


    }


}
