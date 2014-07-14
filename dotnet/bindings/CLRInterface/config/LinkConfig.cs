using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Configuration class for the link layer
    /// </summary>
    public class LinkConfig
    {       
        /// <summary>
        /// Full constructor
        /// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
        /// <param name="numRetry">The number of retry attempts the link will attempt after the initial try</param>
        /// <param name="localAddr">dnp3 address of the local device</param>
        /// <param name="remoteAddr">dnp3 address of the remote device</param>
        /// <param name="timeout">the response timeout in milliseconds for confirmed requests</param>
        public LinkConfig(  bool isMaster,
                            bool useConfirms,
                            System.UInt32 numRetry,
                            System.UInt16 localAddr,
                            System.UInt16 remoteAddr,
                            System.UInt32 timeoutMs)
        {
            this.isMaster = isMaster;
            this.useConfirms = useConfirms;
            this.numRetry = numRetry;
            this.localAddr = localAddr;
            this.remoteAddr = remoteAddr;
            this.timeoutMs = timeoutMs;
        }

        /// <summary>
        /// Defaults constructor
        /// </summary>
        /// <param name="isMaster">true if this layer will be used with a master, false otherwise</param>
        /// <param name="useConfirms">true to use link layer confirmations for all data, false otherwise</param>
        public LinkConfig(bool isMaster, bool useConfirms)
            : this(isMaster, useConfirms, DefaultNumRetries, GetDefaultSourceAddress(isMaster), GetDefaultDestinationAddress(isMaster), DefaultTimeoutMillisconds)
        {

        }

        /// <summary>
        /// Defaults to master
        /// </summary>
        public LinkConfig()
            : this(true, false)
        {

        }

        public static System.UInt16 DefaultOutstationAddress
        {
            get
            {
                return 1024;
            }
        }

        public static System.UInt16 DefaultMasterAddress
        {
            get
            {
                return 1;
            }
        }

        public static System.UInt32 DefaultNumRetries
        {
            get
            {
                return 0;
            }
        }

        public static System.UInt16 GetDefaultSourceAddress(bool isMaster)
        {
            return isMaster ? DefaultMasterAddress : DefaultOutstationAddress;
        }

        public static System.UInt16 GetDefaultDestinationAddress(bool isMaster)
        {
            return isMaster ? DefaultOutstationAddress : DefaultMasterAddress;
        }

        public static System.UInt16 GetDefaultAddress(bool isMaster)
        {
            return isMaster ? DefaultMasterAddress : DefaultOutstationAddress;
        }

        public static System.UInt32 DefaultTimeoutMillisconds
        {
            get
            {
                return 1000;
            }
        }

        /// <summary>
        /// The master/outstation bit set on all messages
        /// </summary>
        public bool isMaster;

        /// <summary>
        /// If true, the link layer will send data requesting confirmation
        /// </summary> 
        public bool useConfirms;

        /// <summary>
        /// The number of retry attempts the link will attempt after the initial try
        /// </summary>
        public System.UInt32 numRetry;

        /// <summary>
        /// dnp3 address of the local device
        /// </summary>
        public System.UInt16 localAddr;

        /// <summary>
        /// dnp3 address of the remote device
        /// </summary>
        public System.UInt16 remoteAddr;

        /// <summary>
        /// the response timeout for confirmed requests
        /// </summary>
        public System.UInt32 timeoutMs;
    }
}
