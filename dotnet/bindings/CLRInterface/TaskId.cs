using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public enum TaskIds : int
    {
        CLEAR_RESTART,
        DISABLE_UNSOLICITED,
        ASSIGN_CLASS,
        STARTUP_INTEGRITY_POLL,
        SERIAL_TIME_SYNC,
        ENABLE_UNSOLICITED,
        EVENT_SCAN
    };

    public class TaskId
    {
        public TaskId(int id, bool isUserAssigned)
        {
            this.id = id;
            this.isUserAssigned = isUserAssigned;
        }

        public int Id
        {
            get
            {
                return id;
            }
        }

        public bool IsUserAssigned
        {
            get
            {
                return isUserAssigned;
            }
        }

        readonly int id;
		readonly bool isUserAssigned;

		
    }
}
