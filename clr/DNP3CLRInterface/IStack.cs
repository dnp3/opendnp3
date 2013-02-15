using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Base interface for masters and outstations
    /// </summary>
    public interface IStack
    {
        /// <summary>
        /// Permanently shutdown the stack and any child objects
        /// </summary>
        void Shutdown();
    }
}
