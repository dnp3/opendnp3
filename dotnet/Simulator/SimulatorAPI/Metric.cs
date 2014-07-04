using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.Simulator.API
{
    public class Metric
    {
        readonly String id;
        readonly String value;

        public Metric(String id, String value)
        {
            this.id = id;
            this.value = value;
        }

        public String Id
        {
            get
            {
                return id;
            }
        }

        public String Value
        {
            get
            {
                return value;
            }
        }
    }
}
