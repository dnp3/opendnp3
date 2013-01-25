using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public interface IChannel
    {      		
		IMaster AddMaster(String loggerId, FilterLevel level, IDataObserver publisher, MasterStackConfig config);

		IOutstation AddOutstation(String loggerId, FilterLevel level, ICommandHandler cmdHandler, SlaveStackConfig config);

        void Shutdown();
    }
}
