using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Interface representing an outstation
    /// </summary>
    public interface IOutstation: IStack
    {
        /// <summary>
        /// 
        /// </summary>
        /// <returns>Interface used to load data into the outstation</returns>
        IDataObserver GetDataObserver();
    }
}
