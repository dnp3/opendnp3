using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Simulator
{
    class CommCounter
    {
        private String id;
        private System.UInt32 count;

        public CommCounter(String id, System.UInt32 count)
        {
            this.id = id;
            this.count = count;
        }

        public String Id
        {
            get
            {
                return id;
            }
        }

        public System.UInt32 Count
        {
            get
            {
                return count;
            }
        }
    }
}
