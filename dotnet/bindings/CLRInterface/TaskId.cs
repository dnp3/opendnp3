using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
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
