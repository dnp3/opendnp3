using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Automatak.Simulator.UI;
using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{   
    class ForwardingLogHandler : ILogHandler
    {
        readonly ILog proxy;

        public ForwardingLogHandler(ILog proxy)
        {
            this.proxy = proxy;            
        }                   
               
        static bool DisplayTimestamp(LogFilter filter)
        {
            switch (filter.Flags)
            {                                     
                case (LogFilters.EVENT):                
                case (LogFilters.ERROR):
                case (LogFilters.WARNING):
                case (LogFilters.INFO):
                case (LogFilters.DEBUG):
                case (LogFilters.LINK_RX):
                case (LogFilters.LINK_TX):
                case (LogFilters.TRANSPORT_RX):
                case (LogFilters.TRANSPORT_TX):
                case (LogFilters.APP_HEADER_RX):
                case (LogFilters.APP_HEADER_TX):
                    return true;  
                default:
                    return false;
            }
        }

        void ILogHandler.Log(LogEntry entry)
        {
            if (DisplayTimestamp(entry.filter))
            {
                proxy.Log(DisplayHint.EVENT, "");
                proxy.LogFull(DisplayHint.EVENT, LogFilters.GetFilterString(entry.filter.Flags), entry.alias, entry.message);                
            }
            else
            {
                proxy.Log(DisplayHint.EVENT, entry.message);
            }        
        }                        
    }
}
