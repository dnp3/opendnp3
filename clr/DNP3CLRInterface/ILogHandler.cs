
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    /// <summary>
    /// A message from the logging framework
    /// </summary>
    public class LogEntry
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="filterLevel">Level enumeration of the message</param>
        /// <param name="loggerName">Logger id that recorded the message</param>
        /// <param name="location">Location in the code</param>
        /// <param name="message">Body of the message</param>
        /// <param name="time">Timestamp on the message</param>
        /// <param name="errorCode">dnp3 error code</param>
        public LogEntry(LogLevel filterLevel, String loggerName, String location, string message, DateTime time, int errorCode)
        {
            this.filterLevel = filterLevel;
            this.loggerName = loggerName;
            this.location = location;
            this.message = message;
            this.time = time;
            this.errorCode = errorCode;
            this.keyValues = new Dictionary<string,string>();
        }

        /// <summary>
        /// Level enumeration of the message
        /// </summary>
        public readonly LogLevel filterLevel;

        /// <summary>
        /// Logger id that recorded the message
        /// </summary>
	    public readonly String loggerName;

        /// <summary>
        /// Location in the code
        /// </summary>
	    public readonly String location;

        /// <summary>
        /// Body of the message
        /// </summary>
	    public readonly String message;

        /// <summary>
        /// Timestamp on the message
        /// </summary>
	    public readonly DateTime time;

        /// <summary>
        /// dnp3 error code
        /// </summary>
	    public readonly int	errorCode;

        /// <summary>
        /// Free-form key-value pairs associated with the message
        /// </summary>
	    public readonly IDictionary<String, String> keyValues;
    }

    /// <summary>
    /// Callback interface to handle log messages
    /// </summary>
    public interface ILogHandler
    {        
	    /// <summary>
	    /// log an error message
	    /// </summary>
	    /// <param name="entry">log entry instance</param>
        void Log(LogEntry entry);	  
    }

    /// <summary>
    /// Singleton log adapter that prints messages to the console
    /// </summary>
    public class PrintingLogAdapter : ILogHandler
    {
        private static readonly PrintingLogAdapter instance = new PrintingLogAdapter();

        public static ILogHandler Instance
        {
            get
            {
                return instance;
            }
        }

        public void Log(LogEntry entry)
        {
            Console.WriteLine(entry.message);
        }

        private PrintingLogAdapter()
        { }
    }
}
