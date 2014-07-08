using System;
using System.Text;

namespace Automatak.Simulator.Commons
{
    public enum DisplayHint
    { 
        EVENT,
        ERROR,
        WARNING,
        INFO,
        ALT1,
        ALT2,
        ALT3,
    };

    public interface ILog
    {
        void Log(DisplayHint hint, string message);

        void LogFull(DisplayHint hint, string filter, string alias, string message);
    }
    
}
