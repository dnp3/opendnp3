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

        /// <summary>
        /// Add a listener for changes to the stack state. All callbacks come from the thread pool.
		/// An immediate callback will be made with the current state.		
        /// </summary>
        /// <param name="listener">Action to call back with the state enumeration </param>
        void AddStateListener(Action<StackState> listener);
    }
}
