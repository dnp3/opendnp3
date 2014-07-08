using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.Simulator.Commons
{
    public class LogMultiplexer : ILog
    {
        readonly IEnumerable<ILog> logs;

        public LogMultiplexer(params ILog[] logs)
        {
            this.logs = new List<ILog>(logs);
        }       

        void ILog.Log(DisplayHint hint, string message)
        {
            foreach (var log in logs)
            {
                log.Log(hint, message);
            }
        }

        void ILog.LogFull(DisplayHint hint, string filter, string alias, string message)
        {
            foreach (var log in logs)
            {
                log.LogFull(hint, filter, alias, message);
            }
        }
    }
}
