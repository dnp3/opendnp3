using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public interface IMasterScan
    {        
        void Demand();

        void AddScanCallback(Action<PollState> callback);
    }
}
