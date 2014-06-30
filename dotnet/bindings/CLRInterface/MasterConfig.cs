using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Configuration information for the dnp3 master
    /// </summary>
    public class MasterConfig
    {

        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public MasterConfig()
        {
            responseTimeout = TimeSpan.FromSeconds(5);
            timeSyncMode = TimeSyncMode.None;
            disableUnsolOnStartup = true;
            unsolClassMask = ClassField.AllEventClasses;
            startupIntergrityClassMask = ClassField.AllClasses;
            integrityOnEventOverflowIIN = true;
            taskRetryPeriod = TimeSpan.FromSeconds(5);
        }

        /// <summary>
        /// Application layer response timeout
        /// </summary>
        public TimeSpan responseTimeout;

        /// <summary>
        /// If true, the master will do time syncs when it sees the time IIN bit from the outstation
        /// </summary>
        public TimeSyncMode timeSyncMode;

        /// <summary>
        /// If true, the master will disable unsol on startup for all 3 classes
        /// </summary>
        public bool disableUnsolOnStartup;

        /// <summary>
        /// Bitwise mask used determine which classes are enabled for unsol, if 0 unsol is not enabled
        /// </summary>
        public ClassField unsolClassMask;

        /// <summary>
        /// Which classes should be requested in a startup integrity scan, defaults to 3/2/1/0
        /// A mask equal to 0 means no startup integrity scan will be performed
        /// </summary>
        public ClassField startupIntergrityClassMask;

        /// <summary>
        /// Defines whether an integrity scan will be performed when the EventBufferOverflow IIN is detected
        /// </summary>
        public bool integrityOnEventOverflowIIN;

        /// <summary>
        /// Time delay beforce retrying a failed task
        /// </summary>
        public TimeSpan taskRetryPeriod;
    }

}
