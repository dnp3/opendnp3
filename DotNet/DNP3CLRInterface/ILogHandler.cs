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

	    /// <summary>
        /// Updates a variable/metric in the system
	    /// </summary>
	    /// <param name="source"></param>
	    /// <param name="varName"></param>
	    /// <param name="value"></param>
	    void SetVar(String source, String varName, int value);

    }
}
