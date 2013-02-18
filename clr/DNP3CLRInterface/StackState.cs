using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Enumeration describing the communication health of a master or outstation
    /// </summary>
    public enum StackState
    {
        /// <summary>
        /// The stack is successfully communicating with the other side
        /// </summary>
        COMMS_UP = 0,
        /// <summary>
        /// The stack is not communicating well with the other side
        /// </summary>
        COMMS_DOWN = 1,
        /// <summary>
        /// The communication health is unknown
        /// </summary>
        UNKNOWN = 2
    };
}
