
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
    public class LogEntry
    {
        public LogEntry(FilterLevel filterLevel, String loggerName, String location, string message, DateTime time, int errorCode)
        {
            this.filterLevel = filterLevel;
            this.loggerName = loggerName;
            this.location = location;
            this.message = message;
            this.time = time;
            this.errorCode = errorCode;
            this.keyValues = new Dictionary<string,string>();
        }

        public readonly FilterLevel filterLevel;
	    public readonly String loggerName;
	    public readonly String location;
	    public readonly String message;
	    public readonly DateTime time;
	    public readonly int	errorCode;
	    public readonly IDictionary<String, String> keyValues;
    }

    public interface ILogHandler
    {        
	    /// <summary>
	    /// logging error messages, etc
	    /// </summary>
	    /// <param name="entry">log entry instance</param>
        void Log(LogEntry entry);	  
    }

    public class PrintingLogAdapter : ILogHandler
    {

        public void Log(LogEntry entry)
        {
            Console.WriteLine(entry.message);
        }       
    }
}
