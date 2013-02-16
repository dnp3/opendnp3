using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Root interface used for all dnp3 applications
    /// </summary>
    public interface IDNP3Manager
    {
     

		/// <summary>
        /// Add a TCP client channel. The channel does not try to connect until you add a stack.
		/// </summary>
        /// <param name="name">An id used for logging purposes</param>
        /// <param name="level">The starting level for logging output</param>
        /// <param name="retryMs">The number of milliseconds in between connection attempts</param>
        /// <param name="address">The address of remote host as a INET address i.e. "127.0.0.1" or name "www.google.com"</param>
        /// <param name="port">The port to make the connection on</param>
        /// <returns>A channel interface</returns>
		IChannel AddTCPClient(String name, LogLevel level, UInt64 retryMs, String address, UInt16 port);

        /// <summary>
        /// Add a TCP server channel. The channel does not try to listen until you add a stack.
        /// </summary>
        /// <param name="name">An id used for logging purposes</param>
        /// <param name="level">The starting level for logging output</param>
        /// <param name="retryMs">The number of milliseconds in between connection attempts</param>
        /// <param name="endpoint">The address that identifies the network adapter to bind i.e. "127.0.0.1" or "0.0.0.0"</param>
        /// <param name="port">The port to listen on</param>
        /// <returns>A channel interface</returns>
		IChannel AddTCPServer(String name, LogLevel level, UInt64 retryMs, String endpoint, UInt16 port);

        /// <summary>
        /// Add a serial channel. The port does not try to open until you add a stack.
        /// </summary>
        /// <param name="name">An id used for logging purposes</param>
        /// <param name="level">The starting level for logging output</param>
        /// <param name="retryMs">The number of milliseconds in between connection attempts</param>
        /// <param name="settings">Configuration struct for the serial port</param>        
        /// <returns>A channel interface</returns>
		IChannel AddSerial(String name, LogLevel level, UInt64 retryMs, SerialSettings settings);

        /// <summary>
        /// Subscribe to all log messages
        /// </summary>
        /// <param name="logHandler">Callback interface</param>
		void AddLogHandler(ILogHandler logHandler);		
    }
}
