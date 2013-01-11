using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace DotNetTestSet
{
    class LogControlAdapter : ILogHandler
    {
        public LogControlAdapter(LogControl ctrl)
        {
            this.logControl = ctrl;
        }

        private LogControl logControl;

        #region ILogHandler Members

        public void Log(LogEntry entry)
        {
            this.logControl.BeginInvoke(new Action(() => logControl.AddLogEntry(entry)));
        }

        public void SetVar(string source, string varName, int value)
        {

        }

        #endregion
    }    
}
