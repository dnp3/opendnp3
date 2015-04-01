using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public class ScanInfo
    {
        public ScanInfo(IMasterScan scan, string type, TimeSpan period, string details)
        {
            this.scan = scan;
            this.type = type;
            this.period = period;
            this.details = details;
        }

        public readonly IMasterScan scan;
        public readonly string type;
        public readonly TimeSpan period;
        public readonly string details;
    }
}
