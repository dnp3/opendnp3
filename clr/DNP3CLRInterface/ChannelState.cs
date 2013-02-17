using System;

namespace DNP3.Interface
{
    /// <summary>
    /// Enumeration for possible states of an IChannel
    /// </summary>
    public enum ChannelState
    {
        /// <summary>
        /// offline and idle
        /// </summary>
        CLOSED,
        /// <summary>
        /// trying to open
        /// </summary>
        OPENING,        
        /// <summary>
        /// waiting to open
        /// </summary>
        WAITING,
        /// <summary>
        /// open
        /// </summary>
        OPEN,
        /// <summary>
        /// stopped and will never do anything again
        /// </summary>
        SHUTDOWN
    }
}
