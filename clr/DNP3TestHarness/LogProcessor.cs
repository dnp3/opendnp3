using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{   
    class LogProcessor : ILogHandler
    {
        private static string timeFormat = "HH:mm:ss.fff";
        private static string logFormat = "  {0,-16}{1,-10}{2,-10}{3}";
       
        private static string GetLogString(DateTime time, uint filters, string id, string message)
        {
            return String.Format(logFormat, time.ToString(timeFormat), LogFilters.GetFilterString(filters), id, message);
        }

        private static IEnumerable<string> GetSystemLogMessages(String message)
        {
            yield return "";
            yield return GetLogString(DateTime.Now, LogFilters.EVENT, "system", message);
        }

        private static string GetBareString(string message)
        {
            return String.Format("  {0}{1}", new String(' ', 36), message);
        }        

        private static IEnumerable<string> GetLogMessages(LogEntry entry)
        {
            if (entry.first)
            {                
                yield return "";
                yield return GetLogString(entry.time, entry.filter.Flags, entry.loggerName, entry.message);
            }
            else
            {                
                yield return GetBareString(entry.message);
            }
        }

        public void LogSystemMessage(String message)
        {
            var msgs = GetSystemLogMessages(message);
            lock (processed)
            {
                foreach(var s in msgs)
                {
                    processed.Enqueue(s);
                }                
            }
        }
     
        public ICollection<string> Flush()
        {
            lock (processed)
            {
                var list = new List<string>(processed);
                processed.Clear();
                return list;
            }
        }

        void ILogHandler.Log(LogEntry entry)
        {            
            lock (processed)
            {
                var lines = GetLogMessages(entry);
                foreach (var s in lines)
                {
                    processed.Enqueue(s);
                }
            }         
        }        

        //private Queue<LogEntry> raw = new Queue<LogEntry>();
        private Queue<string> processed = new Queue<string>();
    }
}
