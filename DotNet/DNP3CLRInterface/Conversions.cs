using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    public class TimeStamp
    {
        // DNP3 uses different epoch than .NET
        private static readonly DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, System.DateTimeKind.Utc);
       
        public static DateTime Convert(Int64 time)        
	    {
            return epoch.Add(TimeSpan.FromMilliseconds(time));           
	    }

        public static Int64 Convert(DateTime time)
	    {
            return (long) time.Subtract(epoch).TotalMilliseconds;
	    }        
    }
}
