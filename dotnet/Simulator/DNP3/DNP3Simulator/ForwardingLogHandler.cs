using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Automatak.Simulator.Commons;
using Automatak.DNP3.Interface;

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
                case(LogFilters.APP_OBJECT_RX):
                case(LogFilters.APP_OBJECT_TX):
                case(LogFilters.LINK_RX_HEX):
                case(LogFilters.LINK_TX_HEX):
                    return false;
                default:
                    return true;
            }
        }

        static DisplayHint GetDisplayHint(LogFilter filter)
        {
            switch (filter.Flags)
            {
                case(LogFilters.ERROR):
                    return DisplayHint.ERROR;
                case(LogFilters.WARNING):
                    return DisplayHint.WARNING;
                case (LogFilters.APP_OBJECT_RX):
                case (LogFilters.APP_OBJECT_TX):
                case (LogFilters.LINK_RX_HEX):
                case (LogFilters.LINK_TX_HEX):
                    return DisplayHint.ALT1;
                default:
                    return DisplayHint.INFO;
            }
        }

        void ILogHandler.Log(LogEntry entry)
        {
            var hint = GetDisplayHint(entry.filter);

            if (DisplayTimestamp(entry.filter))
            {
                proxy.Log(hint, "");
                proxy.LogFull(hint, LogFilters.GetFilterString(entry.filter.Flags), entry.alias, entry.message);                
            }
            else
            {
                proxy.Log(hint, entry.message);
            }        
        }                        
    }
}
