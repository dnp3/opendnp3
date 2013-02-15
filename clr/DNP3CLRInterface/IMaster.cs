using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Interface representing a master station
    /// </summary>
    public interface IMaster: IStack
    {
        /// <summary>
        /// 
        /// </summary>
        /// <returns>Interface used to dispatch command requests</returns>
        ICommandProcessor GetCommandProcessor();
    }
}
