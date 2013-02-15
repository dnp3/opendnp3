using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{    
    /// <summary>
    /// A communication channel to which DNP3 masters / outstation can be attached
    /// </summary>
    public interface IChannel
    {      
        /// <summary>
        /// Adds a master stack to the channel
        /// </summary>
        /// <param name="loggerId">name of the logger that will be assigned to this stack</param>
        /// <param name="level">LogLevel assigned to the logger</param>
        /// <param name="publisher">Where measurements will be sent as they are received from the outstation</param>
        /// <param name="config">configuration information for the master stack</param>
        /// <returns>reference to the created master</returns>
		IMaster AddMaster(String loggerId, LogLevel level, IDataObserver publisher, MasterStackConfig config);

        /// <summary>
        /// Adds an outstation to the channel
        /// </summary>
        /// <param name="loggerId">name of the logger that will be assigned to this stack</param>
        /// <param name="level">LogLevel assigned to the logger</param>
        /// <param name="cmdHandler">where command requests are sent to be handled in application code</param>
        /// <param name="config">configuration information for the outstation stack</param>
        /// <returns>reference to the created master</returns>
		IOutstation AddOutstation(String loggerId, LogLevel level, ICommandHandler cmdHandler, SlaveStackConfig config);

        /// <summary>
        /// Shutdown the channel and all stacks that have been added. Calling shutdown more than once or
        /// continuing to use child objects (masters/outstations) after calling shutdown can cause a failure.     
        /// </summary>
        void Shutdown();
    }
}
