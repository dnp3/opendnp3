using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public interface IChannel
    {      		
		IMaster AddMaster(String loggerId, LogLevel level, IDataObserver publisher, MasterStackConfig config);

		IOutstation AddOutstation(String loggerId, LogLevel level, ICommandHandler cmdHandler, SlaveStackConfig config);

        void Shutdown();
    }
}
